#include <Windows.h>
#include "resource.h"
#include <string.h>

HWND hEdit1;
HWND hEdit2;

CHAR str1[80] = { 0 };
CHAR str2[80] = { 0 };
CONST CHAR szFilter[] = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgUsrProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL LoadTextFileToEdit(HWND hwnd, LPSTR pszFileName);
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR pszFileName);


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

	///////////////   Message loop

	MSG msg; // Переменная сообщение
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (!TranslateAccelerator(msg.hwnd, hAccel, &msg)) // Если сообщение от кнопки акселератора
		{
			TranslateMessage(&msg);     // Если сообщение не от кнопки акселератора
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;

}
/******************************************   WndProc   ********************************************************************************/
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
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, "&Open");
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_SAVE, "&Save");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&About");
		AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&User");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");
		SetMenu(hwnd, hMenu);

		//Adding icons
		HICON hIcon = (HICON)LoadImage(NULL, "Document.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		
		RECT rect;
		GetClientRect(hwnd, &rect);


		//Text editor:
		HWND hEdit = CreateWindowEx(
			WS_EX_CLIENTEDGE, "Edit", "",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			0, 0, rect.right, rect.bottom,
			hwnd,
		    (HMENU)IDC_MAIN_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		
	}
	break;
	case WM_SIZE:
	{
		RECT rcClient;
		GetClientRect(hwnd, &rcClient);
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom,SWP_NOZORDER);

	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) // LOWORD содержит ID сообщения
		{

		case ID_FILE_OPEN:
		{
			//создадим стандартное окно открытия /сохранения файла:
			OPENFILENAME ofn;  // создаем структуру 
			CHAR szFileName[MAX_PATH] = {}; 

			ZeroMemory(&ofn, sizeof(ofn)); // зануление полей структуры

			// Заполняем поля структуры
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd; // определитель родительского объекта
			ofn.lpstrFilter = szFilter; // константа в начале кода
			ofn.lpstrFile = szFileName; // строка куда сохранится путь к файлу
			ofn.nMaxFile = MAX_PATH; // Максимально возможная длина пути 256 байт
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY; // флаги открытия файла
			ofn.lpstrDefExt = "txt"; // разрешение по умолчанию с которым сохраняется файл
			
			if (GetOpenFileName(&ofn)) // Если получилось открыть файл
			{
				//Происходит чето-там
				HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
				LoadTextFileToEdit(hEdit, szFileName);
			}
				
		}
			break;
		case ID_FILE_SAVE:
		{
			OPENFILENAME ofn;  // создаем структуру 
			CHAR szFileName[MAX_PATH] = {};

			ZeroMemory(&ofn, sizeof(ofn)); // зануление полей структуры

			// Заполняем поля структуры
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd; // определитель родительского объекта
			ofn.lpstrFilter = szFilter; // константа в начале кода
			ofn.lpstrFile = szFileName; // строка куда сохранится путь к файлу
			ofn.nMaxFile = MAX_PATH; // Максимально возможная длина пути 256 байт
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY; // флаги открытия файла
			ofn.lpstrDefExt = "txt"; // разрешение по умолчанию с которым сохраняется файл

			if (GetSaveFileName(&ofn)) // Если получилось открыть файл
			{
				//Происходит чето-там
				HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
				SaveTextFileFromEdit(hEdit, szFileName);
			}

		}
			break;
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
		case ID_F1:
			MessageBox(hwnd, "Use key F1!", "Info", MB_OK | MB_ICONINFORMATION);
			DestroyWindow(hwnd);
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
		//MessageBox(hwnd, "От странные, лучше б дверь закрыли","Возмущение", MB_OK | MB_ICONINFORMATION);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}
	return 0;
}
/******************************************   DlgProc   ********************************************************************************/
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
/*******************************************   DlgUsrProc   ****************************************************************************/
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
/*******************************************   LoadTextFileToEdit   ********************************************************************/
BOOL LoadTextFileToEdit(HWND hEdit, LPSTR pszFileName)
{
	BOOL bSuccess = FALSE; // флаг успешности
	HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL); // Обработчик файла
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0xFFFFFFFF) // проверяем не привышает ли максимальный размер
		{
			LPSTR pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1); // Выделяем память для чтения файла
			if (pszFileText)
			{
				DWORD dwRead;
				if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{
					pszFileText[dwFileSize] = 0; // Ставим в конец 0
					if (SetWindowText(hEdit, pszFileText)) bSuccess = TRUE;
				}
				GlobalFree(pszFileText);

			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}
/******************************************* SaveTextFileFromEdit   ********************************************************************/
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR pszFileName)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength = GetWindowTextLength(hEdit);
		if (dwTextLength > 0)
		{
			LPSTR pszText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
			if (pszText != NULL)
			{
				if (GetWindowText(hEdit, pszText, dwTextLength + 1))
				{
					DWORD dwWritten;
					if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL)) bSuccess = TRUE;
				}
				GlobalFree(pszText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}