#include "Lesson8PCH.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	setlocale(LC_ALL, "");

	MainDialog mainDlg;
	return mainDlg.Display(hInstance);

	return 0;
}


