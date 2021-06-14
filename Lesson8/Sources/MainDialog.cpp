#include "Lesson8PCH.h"

MainDialog* gMainDialog = nullptr;

MainDialog::MainDialog() :
	WindowHandle(nullptr),
	Instance(nullptr)
{
	gMainDialog = this;
}

MainDialog::~MainDialog()
{
}

bool MainDialog::Display(HINSTANCE inst)
{
	Instance = inst;

	return DialogBox(inst, MAKEINTRESOURCE(IDD_MAINDIALOG), nullptr, MainDialogProc) ? true : false;
}

INT_PTR CALLBACK MainDialog::MainDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return gMainDialog->MsgProc(hDlg, iMsg, wParam, lParam);
}

INT_PTR MainDialog::MsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	WindowHandle = hDlg;

	if (GetAsyncKeyState(VK_ESCAPE)) {
		EndDialog(hDlg, 0);
		return 0;
	}

	switch (iMsg) {
		case WM_INITDIALOG: {
			return 1;
		}break;
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDEXIT: {
					EndDialog(hDlg, 0);
				}break;
			}
		}break;
	}
	return 0;
}
