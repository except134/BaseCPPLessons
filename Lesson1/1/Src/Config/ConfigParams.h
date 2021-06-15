#pragma once

namespace ee
{
	enum RENDERER
	{
		RENDERER_D3D11,
		RENDERER_D3D12,
		RENDERER_VULKAN,
		RENDERER_MAX
	};

	const std::wstring RendererNames[RENDERER_MAX] = { L"DirectX 11", L"DirectX 12", L"Vulkan" };

	inline std::wstring RendererToString(RENDERER rend)
	{
		if (rend == RENDERER::RENDERER_MAX)
			return RendererNames[0];
		return RendererNames[rend];
	}

	inline RENDERER StringToRenderer(const std::wstring& rend)
	{
		for (int i = 0; i < RENDERER::RENDERER_MAX; i++) {
			if (rend == RendererNames[i])
				return (RENDERER)i;
		}

		return RENDERER::RENDERER_D3D11;
	}

	class ConfigParamsD3D11
	{
	public:
		IDXGIFactory*				DxgiFactory;
		IDXGIAdapter*				DxgiAdapter;
		wchar_t 					AdapterName[MAX_PATH];
		IDXGIOutput*				DxgiOutput;
		wchar_t 					OutputName[MAX_PATH];
		HMONITOR					MonitorHandle;
		DXGI_MODE_DESC				VideoMode;
		wchar_t 					VideoModeName[MAX_PATH];
		D3D_DRIVER_TYPE				DriverType;

		ConfigParamsD3D11() : DxgiFactory(0), DxgiAdapter(0), DxgiOutput(0), MonitorHandle(0),
			DriverType(D3D_DRIVER_TYPE_HARDWARE)
		{}
	};

	class ConfigParamsD3D12
	{
	public:
		IDXGIFactory6*              DxgiFactory;
		IDXGIAdapter1*              DxgiAdapter;
		wchar_t 					AdapterName[MAX_PATH];
		IDXGIOutput*				DxgiOutput;
		wchar_t 					OutputName[MAX_PATH];
		HMONITOR					MonitorHandle;
		DXGI_MODE_DESC1             VideoMode;
		wchar_t 					VideoModeName[MAX_PATH];
		D3D_DRIVER_TYPE				DriverType;

		ConfigParamsD3D12() : DxgiFactory(0), DxgiAdapter(0), DxgiOutput(0), MonitorHandle(0),
			DriverType(D3D_DRIVER_TYPE_HARDWARE) 
		{}
	};

	class ConfigParams
	{
	public:
		RENDERER					Renderer;
		bool                        FullScreen;
		UINT32                      VSyncInterval;
		UINT32                      MultiSamplingCount;
		UINT						MultisamplingQuality;
		HWND						MainWindowHandle;
		int							Width;
		int							Height;
		ConfigParamsD3D11			configParamsD3D11;
		ConfigParamsD3D12			configParamsD3D12;

		ConfigParams() : FullScreen(false), VSyncInterval(0), MultiSamplingCount(1), 
			MainWindowHandle(0), MultisamplingQuality(0), Width(1600), Height(900),
			Renderer(RENDERER::RENDERER_D3D11)
		{}
	};

	extern ConfigParams* gConfigParams;
}




