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
				case IDC_BUTTON11: {
					gameField[0][0].Symbol = GetButtonCaption(IDC_BUTTON11)[0];
					SetButtons();
					return 1;
				}break;
				case IDC_BUTTON12: {
					gameField[0][1].Symbol = GetButtonCaption(IDC_BUTTON12)[0];
					SetButtons();
					return 1;
				}break;
				case IDC_BUTTON13: {
					gameField[0][2].Symbol = GetButtonCaption(IDC_BUTTON13)[0];
					SetButtons();
					return 1;
				}break;
				case IDC_BUTTON21: {
					gameField[1][0].Symbol = GetButtonCaption(IDC_BUTTON21)[0];
					SetButtons();
					return 1;
				}break;
				case IDC_BUTTON22: {
					gameField[1][1].Symbol = GetButtonCaption(IDC_BUTTON22)[0];
					SetButtons();
					return 1;
				}break;
				case IDC_BUTTON23: {
					gameField[1][2].Symbol = GetButtonCaption(IDC_BUTTON23)[0];
					SetButtons();
					return 1;
				}break;
				case IDC_BUTTON31: {
					gameField[2][0].Symbol = GetButtonCaption(IDC_BUTTON31)[0];
					SetButtons();
					return 1;
				}break;
				case IDC_BUTTON32: {
					gameField[2][1].Symbol = GetButtonCaption(IDC_BUTTON32)[0];
					SetButtons();
					return 1;
				}break;
				case IDC_BUTTON33: {
					gameField[2][2].Symbol = GetButtonCaption(IDC_BUTTON33)[0];
					SetButtons();
					return 1;
				}break;
			}
		}break;
	}

	return 0;
}

std::string	MainDialog::GetButtonCaption(int button)
{
	if (!WindowHandle)
		return "F";

	char ret[256];

	GetDlgItemTextA(WindowHandle, IDC_BUTTON11, ret, 256);

	return std::string(ret);
}

void MainDialog::SetButtonCaption(int button, char c)
{
	if (!WindowHandle)
		return;

	char s[2]{};
	s[0] = c;

	SetDlgItemTextA(WindowHandle, IDC_BUTTON11, s);

}

void MainDialog::SetButtons()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (gameField[j][i].Symbol == ' ')
				gameField[j][i].Symbol = 'X';
			else if (gameField[j][i].Symbol == 'X')
				gameField[j][i].Symbol = 'O';
			else if (gameField[j][i].Symbol == 'O')
				gameField[j][i].Symbol = ' ';
		}
	}

	SetButtonCaption(IDC_BUTTON11, gameField[0][0].Symbol);
	SetButtonCaption(IDC_BUTTON12, gameField[0][1].Symbol);
	SetButtonCaption(IDC_BUTTON13, gameField[0][2].Symbol);
	SetButtonCaption(IDC_BUTTON21, gameField[1][0].Symbol);
	SetButtonCaption(IDC_BUTTON22, gameField[1][1].Symbol);
	SetButtonCaption(IDC_BUTTON23, gameField[1][2].Symbol);
	SetButtonCaption(IDC_BUTTON31, gameField[2][0].Symbol);
	SetButtonCaption(IDC_BUTTON32, gameField[2][1].Symbol);
	SetButtonCaption(IDC_BUTTON33, gameField[2][2].Symbol);
}
