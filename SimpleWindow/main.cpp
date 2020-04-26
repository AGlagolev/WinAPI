#include <Windows.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgUsrProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


/***********************************WinMain**************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	//1)Регистрация класса окна:
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
			//int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd,DlgProc);
			switch (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, DlgProc))
			{
			case IDOK: MessageBox(hwnd, "Dialog ended with OK!","Info", MB_OK | MB_ICONINFORMATION); break;
			case IDCANCEL: MessageBox(hwnd, "Dialog ended with CANCEL!","Info", MB_OK | MB_ICONINFORMATION); break;
			case -1: MessageBox(hwnd, "Dialog eFailed!", "Error", MB_OK | MB_ICONERROR); break;
			}
		
		}
			break;
		
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

	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case IDOK:
		{

			EndDialog(hwnd, IDOK);
			MessageBox(hwnd, "Вы ввели", "Вы уверены?", MB_YESNO | MB_ICONQUESTION);
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