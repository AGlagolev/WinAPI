#include <Windows.h>
#include "resource.h"
#include <string.h>
#include <string>
#include<cstdlib>


CHAR str1[80] = { 0 };
CHAR str2[80] = { 0 };
DOUBLE Val1 = 0;
DOUBLE Val2 = 0;
int Metka = 0;

LPCWSTR lpszCurrentText = NULL;
LPCTSTR a = str1;


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DOUBLE GetEditText(HWND hEdit);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);

	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			CHAR szTitle[256] = "Calculator";
			SendMessage(hwnd, WM_SETTEXT, 1, (LPARAM)szTitle);
		}
		break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_BUTTON1:
				{
					SendMessage(hEdit1, WM_CHAR, 0x31, 0);
				}
				break;

				case IDC_BUTTON2:
				{
					SendMessage(hEdit1, WM_CHAR, 0x32, 0);
				}
				break;

				case IDC_BUTTON3:
				{
					SendMessage(hEdit1, WM_CHAR, 0x33, 0);
				}
				break;

				case IDC_BUTTON4:
				{
					SendMessage(hEdit1, WM_CHAR, 0x34, 0);
				}
				break;

				case IDC_BUTTON5:
				{
					SendMessage(hEdit1, WM_CHAR, 0x35, 0);
				}
				break;

				case IDC_BUTTON6:
				{
					SendMessage(hEdit1, WM_CHAR, 0x36, 0);
				}
				break;

				case IDC_BUTTON7:
				{
					SendMessage(hEdit1, WM_CHAR, 0x37, 0);
				}
				break;

				case IDC_BUTTON8:
				{
					SendMessage(hEdit1, WM_CHAR, 0x38, 0);
				}
				break;

				case IDC_BUTTON9:
				{
					SendMessage(hEdit1, WM_CHAR, 0x39, 0);
				}
				break;

				case IDC_BUTTON10:
				{
					SendMessage(hEdit1, WM_CHAR, 0x30, 0);
				}
				break;

				case IDC_BUTTON11:
				{
					Val1 = GetEditText(hEdit1);
					SetWindowText(hEdit1, NULL);
					Metka = 1;
		
				}
				break;

				case IDC_BUTTON12:
				{
					Val1 = GetEditText(hEdit1);
					SetWindowText(hEdit1, NULL);
					Metka = 2;

				}
				break;

				case IDC_BUTTON13:
				{
					Val1 = GetEditText(hEdit1);
					SetWindowText(hEdit1, NULL);
					Metka = 3;
				}
				break;

				case IDC_BUTTON14:
				{
					Val1 = GetEditText(hEdit1);
					SetWindowText(hEdit1, NULL);
					Metka = 4;

				}
				break;

				case IDC_BUTTON15:
				{
					CHAR buff[1024]{};

					Val2 = GetEditText(hEdit1);
					SetWindowText(hEdit1, NULL);
					switch (Metka)
					{
					case 1:sprintf_s(buff, "%.2f", (Val1 + Val2));
					break;

					case 2:sprintf_s(buff, "%.2f", (Val1 - Val2));
					break;

					case 3:sprintf_s(buff, "%.2f", (Val1 * Val2));
					break;

					case 4:sprintf_s(buff, "%.2f", (Val1 / Val2));
					break;

					}
					SetWindowText(hEdit1, buff);

				}
			}
			break;

		case WM_CLOSE:
		    {
				EndDialog(hwnd, 0);
			}
			break;
			
	}

	return FALSE;

	}

	



DOUBLE GetEditText(HWND hEdit)
{
	CHAR buff[1024]{};
	int text = GetWindowText(hEdit, buff, 1024);	
	return atof(buff);
}
