#pragma once

namespace ee
{
	const DXGI_FORMAT DEFAULT_VIDEO_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;

	class GUIObjectW32
	{
	protected:
		INT32  m_IDC;
		HWND*  m_Wnd;

		GUIObjectW32(HWND* wnd, INT32 i) : m_Wnd(wnd), m_IDC(i) {}
		~GUIObjectW32(){};

	public:
		void   Enable()     { if (*m_Wnd == 0)return; EnableWindow(GetDlgItem(*m_Wnd, (int)m_IDC), true); };
		void   Disable()	{ if (*m_Wnd == 0)return; EnableWindow(GetDlgItem(*m_Wnd, (int)m_IDC), false); };
	};

	class GUIComboBoxW32 : public GUIObjectW32
	{
	public:
		GUIComboBoxW32(HWND* wnd, INT32 i) : GUIObjectW32(wnd, i) {}
		~GUIComboBoxW32(){};

		void                        Add(const std::wstring& s)          { if (*m_Wnd == 0)return;           SendDlgItemMessage(*m_Wnd, (int)m_IDC, CB_ADDSTRING, 0, (LPARAM)s.c_str()); };
		void                        Clear()								{ if (*m_Wnd == 0)return;           SendDlgItemMessage(*m_Wnd, (int)m_IDC, CB_RESETCONTENT, 0, 0); };
		void                        SetPos(INT32 idx)					{ if (*m_Wnd == 0)return;           SendDlgItemMessage(*m_Wnd, (int)m_IDC, CB_SETCURSEL, idx, 0); };
		INT32                       GetPos()							{ if (*m_Wnd == 0)return 0;         return (INT32)SendMessage(GetDlgItem(*m_Wnd, (int)m_IDC), CB_GETCURSEL, 0, 0); };
		std::wstring                GetString()							{ if (*m_Wnd == 0)return TEXT("");  wchar_t Str[MAX_PATH]; SendMessage(GetDlgItem(*m_Wnd, (int)m_IDC), CB_GETLBTEXT, GetPos(), (LPARAM)Str); return std::wstring(Str); };
		INT32                       FindString(const std::wstring& str) { if (*m_Wnd == 0)return 0;         return (INT32)SendMessage(GetDlgItem(*m_Wnd, (int)m_IDC), CB_FINDSTRING, 0, (LPARAM)str.c_str()); };
		template<typename T> void   Fill(T& v);
		template<typename T> void   Fill(T& v, bool allres, float dar);
	};

	class GUICheckBoxW32 : public GUIObjectW32
	{
	public:
		GUICheckBoxW32(HWND* wnd, INT32 i) : GUIObjectW32(wnd, i) {}
		~GUICheckBoxW32(){};

		void Set(bool b)	{ if (*m_Wnd == 0)return;       SendMessage(GetDlgItem(*m_Wnd, (int)m_IDC), BM_SETCHECK, (WPARAM)b, 0); };
		bool Checked()      { if (*m_Wnd == 0)return false; return SendMessage(GetDlgItem(*m_Wnd, (int)m_IDC), BM_GETCHECK, 0, 0) == BST_CHECKED; };
	};

	//////////////////////////////////////////////////////////////////////////

	struct DXGI_VIDEO_MODE
	{
		DXGI_MODE_DESC1				Mode;
		std::wstring				GetDescription();
	};

	typedef std::vector<DXGI_VIDEO_MODE>                    DXGI_VIDEO_MODE_TYPE;
	typedef std::vector<DXGI_VIDEO_MODE>::iterator          DXGI_VIDEO_MODE_TYPE_ITE;
	typedef std::vector<DXGI_VIDEO_MODE>::const_iterator    DXGI_VIDEO_MODE_TYPE_CONST_ITE;

	struct DXGI_OUTPUT
	{
		IDXGIOutput*				Output;
		DXGI_VIDEO_MODE_TYPE		Modes;
		DXGI_VIDEO_MODE_TYPE_ITE	SelectedMode;
		DXGI_OUTPUT_DESC			Desc;

		bool						Enumeration();
		std::wstring				GetDescription();
		float						GetDesktopAR();
	};

	typedef std::vector<DXGI_OUTPUT>                        DXGI_OUTPUT_TYPE;
	typedef std::vector<DXGI_OUTPUT>::iterator              DXGI_OUTPUT_TYPE_ITE;
	typedef std::vector<DXGI_OUTPUT>::const_iterator        DXGI_OUTPUT_TYPE_CONST_ITE;

	struct DXGI_AA
	{
		UINT32						Count;
		std::wstring				GetDescription();
	};

	typedef std::vector<DXGI_AA>                            DXGI_AA_TYPE;
	typedef std::vector<DXGI_AA>::iterator                  DXGI_AA_TYPE_ITE;
	typedef std::vector<DXGI_AA>::const_iterator            DXGI_AA_TYPE_CONST_ITE;

	struct DXGI_ADAPTER
	{
		IDXGIAdapter1*				Adapter;
		DXGI_OUTPUT_TYPE			Outputs;
		DXGI_OUTPUT_TYPE_ITE		SelectedOutput;
		DXGI_ADAPTER_DESC1			Desc;
		DXGI_AA_TYPE				AA;
		DXGI_AA_TYPE_ITE			SelectedAA;
		D3D_DRIVER_TYPE				DriverType;
		bool						Enumeration();
		bool						GetAAList();
		bool						CheckForDriverType();
		std::wstring				GetDescription();
	};

	typedef std::vector<DXGI_ADAPTER>                       DXGI_ADAPTER_TYPE;
	typedef std::vector<DXGI_ADAPTER>::iterator             DXGI_ADAPTER_TYPE_ITE;
	typedef std::vector<DXGI_ADAPTER>::const_iterator       DXGI_ADAPTER_TYPE_CONST_ITE;

	struct DXGI_FACTORY
	{
		IDXGIFactory6*				Factory;
		DXGI_ADAPTER_TYPE			Adapters;
		DXGI_ADAPTER_TYPE_ITE		SelectedAdapter;
		bool						Fullscreen;
		bool						VSync;
		bool						Enumeration();
	};

	//////////////////////////////////////////////////////////////////////////

	class ConfigDialog 
	{
	private:
		DXGI_FACTORY          DxgiFactory;
		HINSTANCE             Instance;
		HWND                  WindowHandle;

		std::unique_ptr<GUIComboBoxW32>	cbRenderers;
		std::unique_ptr<GUIComboBoxW32>	cbAdapters;
		std::unique_ptr<GUIComboBoxW32>	cbOutputs;
		std::unique_ptr<GUIComboBoxW32>	cbResolutions;
		std::unique_ptr<GUIComboBoxW32>	cbAA;
		std::unique_ptr<GUICheckBoxW32>	chkbAllres;
		std::unique_ptr<GUICheckBoxW32>	chkbFullscreen;
		std::unique_ptr<GUICheckBoxW32>	chkbDontshow;
		std::unique_ptr<GUICheckBoxW32>	chkbVSync;

		wchar_t                     ConfigFile[MAX_PATH];
		bool                        DontShowDialog;
		bool                        ShowAllResolutions;
		RENDERER					Renderer;

	private:
		static INT_PTR CALLBACK     CfgDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
		bool                        LoadConfig();
		bool                        SaveConfig();
		bool                        Init(bool loadfail, bool withoutcreate = false);
		void                        CheckForFullscreenState();
		void                        ControlsForFullscreen();
		void                        ControlsForWindowed();
		INT_PTR                     MsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	public:
		ConfigDialog();
		~ConfigDialog();

		bool Display(HINSTANCE inst, const std::wstring& cfgFile);
		bool GetParams(ConfigParams& param);
	};
}

#include "ConfigDialogVulkan.h"


