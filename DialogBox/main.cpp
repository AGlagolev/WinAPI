#include<Windows.h>
#include"resource.h"

HWND hEdit1;
HWND hEdit2;

CHAR str1[] = { "Привет " };
CHAR str2[] = { 0 };


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain (HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0); //IDD_DIALOG1 - название диалогового окна в resource.rc
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	

	switch (uMsg)
	{
	case WM_INITDIALOG: // инициализация диалогового окна
	{
	
	// исправлено
	/*hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
	hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);*/

	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, 0, LOWORD(hIcon));
	/*
	SendMessage(hEdit1, WM_SETTEXT, 0, (LPARAM)str1);
	SendMessage(hEdit2, WM_SETTEXT, 0, (LPARAM)str2);*/
	//SetDlgItemText(hwnd, IDC_EDIT1, str1);

	break;
	}
	case WM_COMMAND:  // обработка команд нажатием на кнопки диалогового окна
		switch (LOWORD(wParam))
		{
		case IDOK:
			//MessageBox(hwnd, "Была нажата кнопка ОК ;-)", "Info", MB_OK | MB_ICONINFORMATION);
			GetDlgItemText(hwnd, IDC_EDIT1, str2, 255);
			strcat(str1, str2);
			SetDlgItemText(hwnd, IDC_EDIT2, str1);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		case IDC_COPY: // исправлено
			GetDlgItemText(hwnd, IDC_EDIT1, str2, 255);
			SetDlgItemText(hwnd, IDC_EDIT2, str2);
			/*SendMessage(hEdit1, WM_GETTEXT, 255, (LPARAM)str2);
			SendMessage(hEdit2, WM_SETTEXT, 0, (LPARAM)str2);*/
			break;
		}
		break;
	case WM_CLOSE: // закрытие диалогового окна кнопкой X
		EndDialog(hwnd,0);
		//return TRUE;
		break;
	default:
		break;
	}
	return false;
}