#pragma once

struct GameField
{
	char Symbol{ ' ' };
};

class MainDialog
{
public:
	MainDialog();
	~MainDialog();

	bool					Display(HINSTANCE inst);

	std::string				GetButtonCaption(int button);
	void 					SetButtonCaption(int button, char c);

private:
	HINSTANCE				Instance;
	HWND					WindowHandle;
	static INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	INT_PTR					MsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	GameField				gameField[3][3]{ ' ' };

	void					SetButtons();
};
