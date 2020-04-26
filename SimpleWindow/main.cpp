#include <Windows.h>
#include "resource.h"
#include <string.h>

HWND hEdit1;
HWND hEdit2;

CHAR str1[80] = { 0 };
CHAR str2[80] = { 0 };


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgUsrProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


/***********************************WinMain**************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{




	//1)Регистрация класса окна:
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	CONST CHAR SZ_CLASS_NAME[] = "myWindowClass";
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); // Иконка на панели задач
	//wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16,16,0);// Иконка окна
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszMenuName = NULL;/*MAKEINTRESOURCE(IDR_MENU1); - прикрепление статического меню*/
	wc.lpszClassName = SZ_CLASS_NAME;
	//wc.lpszMenuName = MAKEINTRESOURCE(IDR_ACCELERATOR1);


	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2)Создание окна:

	HWND hwnd = CreateWindowEx
	(
		WS_EX_CLIENTEDGE,
		SZ_CLASS_NAME,
		"Title of my Window",
		WS_OVERLAPPEDWINDOW,
		0,0,640,480,
		NULL,NULL,hInstance,NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window was not created", "Erorr", MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Message loop

	MSG msg; // Переменная сообщение
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return msg.wParam;

	
	BOOL bRet = 0;
	while (bRet = GetMessage(&msg, NULL, 0, 0))
	{
		if (-1 == bRet) break;
		if (!TranslateAccelerator(msg.hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	// Creating menu
	{


		HMENU hMenu = CreateMenu();
		HMENU hSubMenu;

		hSubMenu = CreatePopupMenu();

		AppendMenu(hSubMenu, MF_STRING, ID_FILE_NEW, "&New");
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&About");
		AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&User");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");
		SetMenu(hwnd, hMenu);

		//Adding icons
		HICON hIcon = (HICON)LoadImage(NULL, "Document.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) //LOWORD содержит ID сообщения
		{
		case ID_FILE_EXIT:
			DestroyWindow(hwnd);
			break;
		case ID_HELP_ABOUT:
		{
			
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_USER), hwnd, DlgUsrProc, 0);	
		
		}
			break;
		case ID_HELP_USER:
		{
			//int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd,DlgProc);
			switch (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_USER), hwnd, DlgUsrProc))
			{
			case IDOK: MessageBox(hwnd, "Dialog ended with OK!", "Info", MB_OK | MB_ICONINFORMATION); break;
			case IDCANCEL: MessageBox(hwnd, "Dialog ended with CANCEL!", "Info", MB_OK | MB_ICONINFORMATION); break;
			case -1: MessageBox(hwnd, "Dialog eFailed!", "Error", MB_OK | MB_ICONERROR); break;
			}

		}
		break;
		//case ID_F1:

		}
	
	}
	break;
	case WM_CLOSE:
		if (MessageBox(hwnd, "Вы действительно хотите закрыть окно?", "Вы уверены?", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			DestroyWindow(hwnd);
		}
		break;
	case WM_DESTROY:
		MessageBox(hwnd, "От странные, лучше б дверь закрыли","Возмущение", MB_OK | MB_ICONINFORMATION);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{

	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case IDOK:
		{
			EndDialog(hwnd, IDOK);
			break;
		}
		case IDCANCEL:
		{
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		}
	}
	break;
	case WM_CLOSE:
	{
	
	}
	break;
	default: return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK DlgUsrProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
		hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);
		SendMessage(hEdit1, WM_SETTEXT, 0, (LPARAM)str1);
		SendMessage(hEdit2, WM_SETTEXT, 0, (LPARAM)str1);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case IDOK:
		{
			SendMessage(hEdit1, WM_GETTEXT, 255, (LPARAM)str1);
			SendMessage(hEdit2, WM_GETTEXT, 255, (LPARAM)str2);
						
			strcat_s(str1, " ");
			strcat_s(str1,str2);
			EndDialog(hwnd, IDOK);
			MessageBox(hwnd, str1, "Вы ввели", MB_YESNO | MB_ICONQUESTION);
			break;
		}
		case IDCANCEL:
		{
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		}
	}
	break;
	case WM_CLOSE:
	{
		EndDialog(hwnd,NULL);
	}
	break;
	default: return FALSE;
	}
	return TRUE;
}