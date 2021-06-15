#include "ExceptEnginePCH.h"

namespace ee
{
	ConfigParams lgConfigParams;
	ConfigParams* gConfigParams = &lgConfigParams;

	template<typename T>
	void GUIComboBoxW32::Fill(T& v)
	{
		Clear();
		size_t sz = v.size();
		for (size_t i = 0; i < sz; i++) {
			Add(v[i].GetDescription());
		}
		SetPos(0);
	}

	template<typename T>
	void GUIComboBoxW32::Fill(T& v, bool allres, float dar)
	{
		Clear();
		size_t sz = v.size();
		for (size_t i = 0; i < sz; i++) {
			if (allres) {
				Add(v[i].GetDescription());
			} else {
				LONG32 width = v[i].Mode.Width;
				LONG32 height = v[i].Mode.Height;
				float ar = float(width) / float(height);
				if (fabs(dar - ar) < 0.05f)
					Add(v[i].GetDescription());
			}
		}
		SetPos(0);
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T>
	static std::wstring BuildResolutionString(const T& Desc)
	{
		wchar_t tmp[MAX_PATH];
		swprintf_s(tmp, L"%dx%d @ %d Hz", Desc.Width, Desc.Height, UINT32(Desc.RefreshRate.Numerator / Desc.RefreshRate.Denominator));
		return std::wstring(tmp);
	}

	template<typename V, typename T>
	static T GetByName(V& v, const std::wstring& s)
	{
		for (T i = v.begin(); i != v.end(); ++i) {
			if (i->GetDescription() == s) {
				return i;
			}
		}
		return v.end();
	}

	std::wstring DXGI_VIDEO_MODE::GetDescription()
	{
		return BuildResolutionString(Mode);
	}

	bool DXGI_OUTPUT::Enumeration()
	{
		Modes.clear();

		Output->GetDesc(&Desc);

		UINT flags = 0;
		UINT num = 0;

		Output->GetDisplayModeList(DEFAULT_VIDEO_FORMAT, flags, &num, 0);
		auto pDescs = std::make_unique<DXGI_MODE_DESC[]>(num);
		Output->GetDisplayModeList(DEFAULT_VIDEO_FORMAT, flags, &num, pDescs.get());

		for (size_t i = 0; i < num; i++) {
			bool found = false;
			for (size_t j = 0; j < Modes.size(); j++) {
				bool width = Modes[j].Mode.Width == pDescs[i].Width;
				bool height = Modes[j].Mode.Height == pDescs[i].Height;
				bool rr = (Modes[j].Mode.RefreshRate.Numerator == pDescs[i].RefreshRate.Numerator && Modes[j].Mode.RefreshRate.Denominator == pDescs[i].RefreshRate.Denominator);
				if (width && height && rr) {
					found = true;
				}
			}
			if (!found) {
				DXGI_VIDEO_MODE vm;
				vm.Mode.Format = pDescs[i].Format;
				vm.Mode.Height = pDescs[i].Height;
				vm.Mode.Width = pDescs[i].Width;
				vm.Mode.Scaling = pDescs[i].Scaling;
				vm.Mode.ScanlineOrdering = pDescs[i].ScanlineOrdering;
				vm.Mode.RefreshRate.Denominator = pDescs[i].RefreshRate.Denominator;
				vm.Mode.RefreshRate.Numerator = pDescs[i].RefreshRate.Numerator;
				Modes.push_back(vm);
			}
		}

		std::sort(Modes.begin(), Modes.end(), [](auto& m1, auto& m2) {return(m1.Mode.Width > m2.Mode.Width && m1.Mode.Height > m2.Mode.Height); });

		SelectedMode = Modes.begin();

		return true;
	}

	std::wstring DXGI_OUTPUT::GetDescription()
	{
		return Desc.DeviceName;
	}

	float DXGI_OUTPUT::GetDesktopAR()
	{
		LONG32 Width = Desc.DesktopCoordinates.right - Desc.DesktopCoordinates.left;
		LONG32 Height = Desc.DesktopCoordinates.bottom - Desc.DesktopCoordinates.top;
		return float(Width) / float(Height);
	}

	std::wstring DXGI_AA::GetDescription()
	{
		return std::to_wstring(Count);
	}

	bool DXGI_ADAPTER::Enumeration()
	{
		if (!CheckForDriverType())
			return false;
		if (!GetAAList())
			return false;

		Adapter->GetDesc1(&Desc);

		Outputs.clear();

		UINT OutputID = 0;
		IDXGIOutput* pOutput = 0;
		while (Adapter->EnumOutputs(OutputID, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			DXGI_OUTPUT output;
			output.Output = pOutput;
			Outputs.push_back(output);
			if (!Outputs[OutputID].Enumeration())
				return false;
			++OutputID;
		}
		SelectedOutput = Outputs.begin();
		return true;
	}

	bool DXGI_ADAPTER::GetAAList()
	{
		AA.clear();
		ID3D12Device* pD3D12Device = nullptr;	

		if (D3D12CreateDevice(Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pD3D12Device)) == S_OK) {
			for (int i = 1; i <= D3D12_MAX_MULTISAMPLE_SAMPLE_COUNT; ++i) {
				D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
				msQualityLevels.Format = DEFAULT_VIDEO_FORMAT;
				msQualityLevels.SampleCount = i;
				msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
				msQualityLevels.NumQualityLevels = 0;
				if (SUCCEEDED(pD3D12Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)))) {
					if (msQualityLevels.NumQualityLevels > 0) {
						DXGI_AA a;
						a.Count = i;
						AA.push_back(a);
					}
				}
			}
			SafeRelease(pD3D12Device);
			return true;
		}
		return false;
	}

	bool DXGI_ADAPTER::CheckForDriverType()
	{
		DriverType = D3D_DRIVER_TYPE_HARDWARE;
		ID3D12Device* pD3D12Device = nullptr;

		Adapter->GetDesc1(&Desc);

		if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			return false;
		}

		HRESULT hr = D3D12CreateDevice(Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pD3D12Device));
		if (hr != S_OK) 	{
			LOG.Write(L"Error creating D3D12 device while running configurator");
			return false;
		}

		SafeRelease(pD3D12Device);
		return true;
	}

	std::wstring DXGI_ADAPTER::GetDescription()
	{
		return Desc.Description;
	}

	bool DXGI_FACTORY::Enumeration()
	{
		Fullscreen = false;
		VSync = true;

		HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&Factory));
		if (FAILED(hr)) {
			LOG.Write(L"Cannot create DXGI Factory");
			return false;
		}

		Adapters.clear();

		UINT AdapterID = 0;
		IDXGIAdapter1* pAdapter = nullptr;
		while (Factory->EnumAdapters1(AdapterID, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
			DXGI_ADAPTER adapter;
			adapter.Adapter = pAdapter;
			Adapters.push_back(adapter);
			if (!Adapters[AdapterID].Enumeration())
				continue;
			++AdapterID;
			break;
		}
		SelectedAdapter = Adapters.begin();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////

	ConfigDialog* gConfigDialog = nullptr;

	ConfigDialog::ConfigDialog() : 
		WindowHandle(nullptr),
		Instance(nullptr),
		DontShowDialog(false),
		ShowAllResolutions(false)
	{
		gConfigDialog = this;
	}

	ConfigDialog::~ConfigDialog()
	{
	}

	bool ConfigDialog::Display(HINSTANCE inst, const std::wstring& cfgFile)
	{
		Instance = inst;
		wcscpy_s(ConfigFile, cfgFile.c_str());

		if (!DxgiFactory.Enumeration())
			return false;

		return DialogBox(inst, MAKEINTRESOURCE(IDD_CONFIG_DIALOG), nullptr, CfgDialogProc) ? true : false;
	}

	const std::wstring CONFIG_GENERAL_SECTION = L"General";
	const std::wstring CONFIG_GENERAL_RENDERER = L"Renderer";
	const std::wstring CONFIG_GRAPHICS_ADAPTER = L"Adapter";
	const std::wstring CONFIG_GRAPHICS_OUTPUT = L"Output";
	const std::wstring CONFIG_GRAPHICS_RES = L"Resolution";
	const std::wstring CONFIG_GRAPHICS_AA = L"Multisampling";
	const std::wstring CONFIG_GRAPHICS_FULLSCR = L"Fullscreen";
	const std::wstring CONFIG_GRAPHICS_VSYNC = L"VSync";
	const std::wstring CONFIG_GRAPHICS_DONTSHOW = L"DontShow";
	const std::wstring CONFIG_GRAPHICS_DRVTYPE = L"DriverType";
	const std::wstring CONFIG_GRAPHICS_ALLRES = L"AllRes";

	bool ConfigDialog::LoadConfig()
	{
		std::wstring renderer = IniFile::GetString(CONFIG_GENERAL_RENDERER, CONFIG_GENERAL_SECTION, ConfigFile);
		std::wstring graphicsSection = RendererNames[0];

		if (renderer != L"") {
			graphicsSection = renderer;
		} 

		Renderer = StringToRenderer(graphicsSection);

		std::wstring adapter= IniFile::GetString(CONFIG_GRAPHICS_ADAPTER, graphicsSection, ConfigFile);
		std::wstring output	= IniFile::GetString(CONFIG_GRAPHICS_OUTPUT, graphicsSection, ConfigFile);
		std::wstring res	= IniFile::GetString(CONFIG_GRAPHICS_RES, graphicsSection, ConfigFile);
		std::wstring aa		= IniFile::GetString(CONFIG_GRAPHICS_AA, graphicsSection, ConfigFile);
		bool fullscr		= IniFile::GetBool(CONFIG_GRAPHICS_FULLSCR, graphicsSection, ConfigFile);
		bool vsync			= IniFile::GetBool(CONFIG_GRAPHICS_VSYNC, graphicsSection, ConfigFile);
		bool dontshow		= IniFile::GetBool(CONFIG_GRAPHICS_DONTSHOW, graphicsSection, ConfigFile);
		bool allres			= IniFile::GetBool(CONFIG_GRAPHICS_ALLRES, graphicsSection, ConfigFile);
		UINT_PTR drvtype	= IniFile::GetInt(CONFIG_GRAPHICS_DRVTYPE, graphicsSection, ConfigFile);

		DXGI_ADAPTER_TYPE_ITE adapteriter = GetByName<DXGI_ADAPTER_TYPE, DXGI_ADAPTER_TYPE_ITE>(DxgiFactory.Adapters, adapter);
		if (adapteriter == DxgiFactory.Adapters.end())
			return false;
		DxgiFactory.SelectedAdapter = adapteriter;

		DXGI_AA_TYPE_ITE aaiter = GetByName<DXGI_AA_TYPE, DXGI_AA_TYPE_ITE>(DxgiFactory.SelectedAdapter->AA, aa);
		if (aaiter == DxgiFactory.SelectedAdapter->AA.end())
			return false;
		DxgiFactory.SelectedAdapter->SelectedAA = aaiter;

		DXGI_OUTPUT_TYPE_ITE outputiter = GetByName<DXGI_OUTPUT_TYPE, DXGI_OUTPUT_TYPE_ITE>(DxgiFactory.SelectedAdapter->Outputs, output);
		if (outputiter == DxgiFactory.SelectedAdapter->Outputs.end())
			return false;
		DxgiFactory.SelectedAdapter->SelectedOutput = outputiter;

		DXGI_VIDEO_MODE_TYPE_ITE videomodeiter = GetByName<DXGI_VIDEO_MODE_TYPE, DXGI_VIDEO_MODE_TYPE_ITE>(DxgiFactory.SelectedAdapter->SelectedOutput->Modes, res);
		if (videomodeiter == DxgiFactory.SelectedAdapter->SelectedOutput->Modes.end())
			return false;
		DxgiFactory.SelectedAdapter->SelectedOutput->SelectedMode = videomodeiter;

		DxgiFactory.Fullscreen = fullscr;
		DxgiFactory.VSync = vsync;
		DxgiFactory.SelectedAdapter->DriverType = static_cast<D3D_DRIVER_TYPE>(drvtype);
		DontShowDialog = dontshow;
		ShowAllResolutions = allres;

		return true;
	}

	bool ConfigDialog::SaveConfig()
	{
		INT32 rend = cbRenderers->GetPos();
		Renderer = (RENDERER)rend;

		std::wstring graphicsSection = cbRenderers->GetString();

		IniFile::SetString(CONFIG_GENERAL_RENDERER, graphicsSection, CONFIG_GENERAL_SECTION, ConfigFile);

		IniFile::SetString(CONFIG_GRAPHICS_ADAPTER, cbAdapters->GetString(), graphicsSection, ConfigFile);
		IniFile::SetString(CONFIG_GRAPHICS_OUTPUT, cbOutputs->GetString(), graphicsSection, ConfigFile);
		IniFile::SetString(CONFIG_GRAPHICS_RES, cbResolutions->GetString(), graphicsSection, ConfigFile);
		IniFile::SetString(CONFIG_GRAPHICS_AA, cbAA->GetString(), graphicsSection, ConfigFile);
		IniFile::SetBool(CONFIG_GRAPHICS_FULLSCR, chkbFullscreen->Checked(), graphicsSection, ConfigFile);
		IniFile::SetBool(CONFIG_GRAPHICS_VSYNC, chkbVSync->Checked(), graphicsSection, ConfigFile);
		IniFile::SetBool(CONFIG_GRAPHICS_DONTSHOW, chkbDontshow->Checked(), graphicsSection, ConfigFile);
		IniFile::SetInt(CONFIG_GRAPHICS_DRVTYPE, DxgiFactory.SelectedAdapter->DriverType, graphicsSection, ConfigFile);
		IniFile::SetBool(CONFIG_GRAPHICS_ALLRES, chkbAllres->Checked(), graphicsSection, ConfigFile);
		return LoadConfig();
	}

	bool ConfigDialog::Init(bool loadfail, bool withoutcreate)
	{
		if (!withoutcreate) {
			cbRenderers = std::make_unique<GUIComboBoxW32>(&WindowHandle, IDC_RENDERER_COMBOBOX);

			cbAdapters = std::make_unique<GUIComboBoxW32>(&WindowHandle, IDC_ADAPTER_COMBOBOX);
			cbOutputs = std::make_unique<GUIComboBoxW32>(&WindowHandle, IDC_DISPLAY_COMBOBOX);
			cbResolutions = std::make_unique<GUIComboBoxW32>(&WindowHandle, IDC_RESOLUTION_COMBOBOX);
			cbAA = std::make_unique<GUIComboBoxW32>(&WindowHandle, IDC_AA_COMBOBOX);

			chkbAllres = std::make_unique<GUICheckBoxW32>(&WindowHandle, IDC_CB_ALLRES);
			chkbFullscreen = std::make_unique<GUICheckBoxW32>(&WindowHandle, IDC_CB_FULLSCREEN);
			chkbDontshow = std::make_unique<GUICheckBoxW32>(&WindowHandle, IDC_CB_DONTSHOW);
			chkbVSync = std::make_unique<GUICheckBoxW32>(&WindowHandle, IDC_CB_VSYNC);
		}

		cbRenderers->Clear();
		for(auto s: RendererNames)
			cbRenderers->Add(s);
		cbRenderers->SetPos(cbRenderers->FindString(RendererToString(Renderer)));

		cbAdapters->Fill<DXGI_ADAPTER_TYPE>(DxgiFactory.Adapters);
		cbOutputs->Fill<DXGI_OUTPUT_TYPE>(DxgiFactory.SelectedAdapter->Outputs);
		cbResolutions->Fill<DXGI_VIDEO_MODE_TYPE>(DxgiFactory.SelectedAdapter->SelectedOutput->Modes, ShowAllResolutions, DxgiFactory.SelectedAdapter->SelectedOutput->GetDesktopAR());
		cbAA->Fill<DXGI_AA_TYPE>(DxgiFactory.SelectedAdapter->AA);

		chkbFullscreen->Set(DxgiFactory.Fullscreen);
		chkbVSync->Set(DxgiFactory.VSync);
		chkbDontshow->Set(DontShowDialog);
		chkbAllres->Set(ShowAllResolutions);

		CheckForFullscreenState();

		if (loadfail) {
			return true;
		}

		cbRenderers->SetPos(cbRenderers->FindString(RendererToString(Renderer)));
		cbAdapters->SetPos(cbAdapters->FindString(DxgiFactory.SelectedAdapter->GetDescription()));
		cbAA->SetPos(cbAA->FindString(DxgiFactory.SelectedAdapter->SelectedAA->GetDescription()));
		cbOutputs->SetPos(cbOutputs->FindString(DxgiFactory.SelectedAdapter->SelectedOutput->GetDescription()));
		cbResolutions->SetPos(cbResolutions->FindString(DxgiFactory.SelectedAdapter->SelectedOutput->SelectedMode->GetDescription()));

		return true;
	}

	INT_PTR CALLBACK ConfigDialog::CfgDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		return gConfigDialog->MsgProc(hDlg, iMsg, wParam, lParam);
	}

	INT_PTR ConfigDialog::MsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowHandle = hDlg;

		if (GetAsyncKeyState(VK_ESCAPE)) {
			EndDialog(hDlg, 0);
			return 0;
		}

		switch (iMsg) {
			case WM_INITDIALOG: {
				bool cfgloaded = LoadConfig();
				Init(!cfgloaded);
				if (chkbDontshow->Checked() && cfgloaded) {
					EndDialog(hDlg, 1);
				}
				return 1;
			}break;
			case WM_COMMAND: {
				switch (LOWORD(wParam)) {
					case IDC_BTN_OK: {
						SaveConfig();
						EndDialog(hDlg, 1);
					}break;
					case IDC_BTN_CANCEL: {
						EndDialog(hDlg, 0);
					}break;
					case IDC_CB_FULLSCREEN: {
						if (HIWORD(wParam) == BN_CLICKED) {
							CheckForFullscreenState();
						}
					}break;
					case IDC_CB_ALLRES: {
						if (HIWORD(wParam) == BN_CLICKED) {
							cbResolutions->Fill<DXGI_VIDEO_MODE_TYPE>(DxgiFactory.SelectedAdapter->SelectedOutput->Modes, chkbAllres->Checked(), DxgiFactory.SelectedAdapter->SelectedOutput->GetDesktopAR());
						}
					}break;
					case IDC_ADAPTER_COMBOBOX: {
						if (HIWORD(wParam) == CBN_SELCHANGE) {
							DXGI_ADAPTER_TYPE_ITE adapteriter = GetByName<DXGI_ADAPTER_TYPE, DXGI_ADAPTER_TYPE_ITE>(DxgiFactory.Adapters, cbAdapters->GetString());
							if (adapteriter == DxgiFactory.Adapters.end())
								return false;
							DxgiFactory.SelectedAdapter = adapteriter;
							cbOutputs->Fill<DXGI_OUTPUT_TYPE>(DxgiFactory.SelectedAdapter->Outputs);
							cbResolutions->Fill<DXGI_VIDEO_MODE_TYPE>(DxgiFactory.SelectedAdapter->SelectedOutput->Modes, chkbAllres->Checked(), DxgiFactory.SelectedAdapter->SelectedOutput->GetDesktopAR());
							cbAA->Fill<DXGI_AA_TYPE>(DxgiFactory.SelectedAdapter->AA);
						}
					}break;
					case IDC_DISPLAY_COMBOBOX: {
						if (HIWORD(wParam) == CBN_SELCHANGE) {
							DXGI_OUTPUT_TYPE_ITE outputiter = GetByName<DXGI_OUTPUT_TYPE, DXGI_OUTPUT_TYPE_ITE>(DxgiFactory.SelectedAdapter->Outputs, cbOutputs->GetString());
							if (outputiter == DxgiFactory.SelectedAdapter->Outputs.end())
								return false;
							DxgiFactory.SelectedAdapter->SelectedOutput = outputiter;
							cbResolutions->Fill<DXGI_VIDEO_MODE_TYPE>(DxgiFactory.SelectedAdapter->SelectedOutput->Modes, chkbAllres->Checked(), DxgiFactory.SelectedAdapter->SelectedOutput->GetDesktopAR());
						}
					}break;
					case IDC_RESOLUTION_COMBOBOX: {
						if (HIWORD(wParam) == CBN_SELCHANGE) {
							DXGI_VIDEO_MODE_TYPE_ITE videomodeiter = GetByName<DXGI_VIDEO_MODE_TYPE, DXGI_VIDEO_MODE_TYPE_ITE>(DxgiFactory.SelectedAdapter->SelectedOutput->Modes, cbResolutions->GetString());
							if (videomodeiter == DxgiFactory.SelectedAdapter->SelectedOutput->Modes.end())
								return false;
							DxgiFactory.SelectedAdapter->SelectedOutput->SelectedMode = videomodeiter;
						}
					}break;
					case IDC_AA_COMBOBOX: {
						if (HIWORD(wParam) == CBN_SELCHANGE) {
							DXGI_AA_TYPE_ITE aaiter = GetByName<DXGI_AA_TYPE, DXGI_AA_TYPE_ITE>(DxgiFactory.SelectedAdapter->AA, cbAA->GetString());
							if (aaiter == DxgiFactory.SelectedAdapter->AA.end())
							   return false;
							DxgiFactory.SelectedAdapter->SelectedAA = aaiter;
						}
					}break;
					case IDC_RENDERER_COMBOBOX: {
						if (HIWORD(wParam) == CBN_SELCHANGE) {
							INT32 rend = cbRenderers->GetPos();
							Renderer = (RENDERER)rend;
							std::wstring graphicsSection = cbRenderers->GetString();
							IniFile::SetString(CONFIG_GENERAL_RENDERER, graphicsSection, CONFIG_GENERAL_SECTION, ConfigFile);
							LoadConfig();
							Init(false, true);
						}
					}break;
				}
			}break;
		}
		return 0;
	}

	void ConfigDialog::CheckForFullscreenState()
	{
		bool lr = chkbFullscreen->Checked();
		if (lr) {
			ControlsForFullscreen();
		} else {
			ControlsForWindowed();
		}
		DxgiFactory.Fullscreen = lr;
	}

	void ConfigDialog::ControlsForFullscreen()
	{
		cbAdapters->Enable();
		cbOutputs->Enable();
		cbResolutions->Enable();
		chkbAllres->Enable();
	}

	void ConfigDialog::ControlsForWindowed()
	{
		cbAdapters->Enable();
		cbOutputs->Enable();
		cbResolutions->Enable();
		chkbAllres->Enable();
/*
		m_cbAdapters->Disable();
		m_cbOutputs->Disable();
		m_cbResolutions->Disable();
		m_chkbAllres->Disable();
*/
	}

	bool ConfigDialog::GetParams(ConfigParams& param)
	{
		param.Renderer = Renderer;
		if (Renderer == RENDERER::RENDERER_D3D11) {
/*
			param.configParamsD3D11.DxgiFactory = DxgiFactory.Factory;
			param.configParamsD3D11.DxgiAdapter = DxgiFactory.SelectedAdapter->Adapter;
			wcscpy_s(param.configParamsD3D11.AdapterName, MAX_PATH, DxgiFactory.SelectedAdapter->GetDescription().c_str());
			param.configParamsD3D11.DriverType = DxgiFactory.SelectedAdapter->DriverType;
			param.configParamsD3D11.DxgiOutput = DxgiFactory.SelectedAdapter->SelectedOutput->Output;
			wcscpy_s(param.configParamsD3D11.OutputName, MAX_PATH, DxgiFactory.SelectedAdapter->SelectedOutput->GetDescription().c_str());
			param.configParamsD3D11.MonitorHandle = DxgiFactory.SelectedAdapter->SelectedOutput->Desc.Monitor;
			param.configParamsD3D11.VideoMode = DxgiFactory.SelectedAdapter->SelectedOutput->SelectedMode->Mode;
			wcscpy_s(param.configParamsD3D11.VideoModeName, MAX_PATH, DxgiFactory.SelectedAdapter->SelectedOutput->SelectedMode->GetDescription().c_str());
*/
		}
		else if (Renderer == RENDERER::RENDERER_D3D12) {
			param.configParamsD3D12.DxgiFactory = DxgiFactory.Factory;
			param.configParamsD3D12.DxgiAdapter = DxgiFactory.SelectedAdapter->Adapter;
			wcscpy_s(param.configParamsD3D12.AdapterName, MAX_PATH, DxgiFactory.SelectedAdapter->GetDescription().c_str());
			param.configParamsD3D12.DriverType = DxgiFactory.SelectedAdapter->DriverType;
			param.configParamsD3D12.DxgiOutput = DxgiFactory.SelectedAdapter->SelectedOutput->Output;
			wcscpy_s(param.configParamsD3D12.OutputName, MAX_PATH, DxgiFactory.SelectedAdapter->SelectedOutput->GetDescription().c_str());
			param.configParamsD3D12.MonitorHandle = DxgiFactory.SelectedAdapter->SelectedOutput->Desc.Monitor;
			param.configParamsD3D12.VideoMode = DxgiFactory.SelectedAdapter->SelectedOutput->SelectedMode->Mode;
			wcscpy_s(param.configParamsD3D12.VideoModeName, MAX_PATH, DxgiFactory.SelectedAdapter->SelectedOutput->SelectedMode->GetDescription().c_str());
		}
		param.FullScreen = DxgiFactory.Fullscreen;
		param.VSyncInterval = DxgiFactory.VSync ? 1 : 0;
		param.MultiSamplingCount = DxgiFactory.SelectedAdapter->SelectedAA->Count;

		LOG.Write(L" Using next parameters:\n");
		LOG.Write(L"        Renderer:           %s\n", RendererNames[Renderer].c_str());
		LOG.Write(L"      Video mode:           %s\n", BuildResolutionString(param.configParamsD3D12.VideoMode).c_str());
		LOG.Write(L"     Multisample:           %d\n", param.MultiSamplingCount);

		return true;
	}
}
