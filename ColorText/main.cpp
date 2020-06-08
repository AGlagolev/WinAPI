#include <Windows.h>
#include"resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, RGB(0, 0, 178));
		SetBkMode(hdc,TRANSPARENT);
		return (int)GetStockObject(NULL_BRUSH);
		
		//

	}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}