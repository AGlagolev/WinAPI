#include<Windows.h>
#include"resource.h"


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain (HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0); //IDD_DIALOG1 - �������� ����������� ���� � resource.rc
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG: // ������������� ����������� ����
		break;
	case WM_COMMAND:  // ��������� ������ �������� �� ������ ����������� ����
		switch (LOWORD(wParam))
		{
		case IDOK:
			MessageBox(hwnd, "���� ������ ������ �� ;-)", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE: // �������� ����������� ���� ������� X
		EndDialog(hwnd,0);
		//return TRUE;
		break;
	default:
		break;
	}
	return false;
}