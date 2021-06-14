#pragma once

class MainDialog
{
public:
	MainDialog();
	~MainDialog();

	bool					Display(HINSTANCE inst);

private:
	HINSTANCE				Instance;
	HWND					WindowHandle;
	static INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	INT_PTR					MsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
};
