#include<Windows.h>
#include"resource.h"
#include <string>




HWND hCombo;

CHAR string1[] = "This";
CHAR string2[] = "is";
CHAR string3[] = "my";
CHAR string4[] = "first";
CHAR string5[] = "combo";
CHAR string6[] = "box";


BOOL CALLBACK DlgProc(HWND hwnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON,1,(LPARAM)hIcon);
		hCombo = GetDlgItem(hwnd, IDC_COMBO1);

		//Добавляем строки в ComboBox

		std::string a[6] = { string1, string2, string3, string4, string5, string6 };
		CHAR Mass[100];
		
		for (int i = 0; i < sizeof(a)/sizeof(std::string); i++)
		{
			strcpy_s(Mass, a[i].c_str());

		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)Mass);
		}
	}
		break;
	case WM_COMMAND:
	{
		
		
		switch (LOWORD(wParam))
		{
		case IDC_COMBO1:

			/*if (HIWORD(wParam) == CBN_SELENDOK)
			{
				int i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
				SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)bufer);
				MessageBox(hwnd, bufer, "Выбраный пункт выпадающего списка", MB_OK | MB_ICONINFORMATION);
			}*/
			break;


		case IDOK:
			
			
			{
			    CHAR bufer[25]{};
				int i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
				SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)bufer);
				MessageBox(hwnd, bufer, "Выбраный пункт выпадающего списка", MB_OK | MB_ICONINFORMATION);
			}
			//MessageBox(hwnd, "Hello", "Hi", MB_OK | MB_ICONINFORMATION);
			break;

		case IDCANCEL:
			EndDialog(hwnd, 0);
			MessageBox(hwnd, "Bye", "Bye", MB_OK | MB_ICONINFORMATION);
			break;
		}
	}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	
	}
	return FALSE;
}