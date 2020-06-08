#include <Windows.h>
#include "resource.h"
#include <string.h>
#include <string>
//Для установки панели инструментов впроект в свойствах проекта ->linker -> Input -> Additional Dependensive добавляем comctl32.lib;
#include<CommCtrl.h> // Добавляется только при выполненнии условий предведущей строки

HWND hEdit1;
HWND hEdit2;

CHAR str1[80] = { 0 };
CHAR str2[80] = { 0 };
LPSTR lpszCurrentText = NULL;
CHAR szPath[MAX_PATH] = {};
CHAR szSize[256] = {};

CONST CHAR szFilter[] = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
HFONT g_hFont;
COLORREF g_rgbText;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgUsrProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL LoadTextFileToEdit(HWND hwnd, LPSTR pszFileName);
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR pszFileName);
BOOL Compare(HWND hEdit);
VOID DoFileSaveAS(HWND hwnd);
BOOL __stdcall DoFileOpen(HWND hwnd);
BOOL __stdcall DoFileSave(HWND hwnd);
VOID SetWindowsTitle(HWND hEdit);
BOOL __stdcall DoFileNew(HWND hwnd)
{
	HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
	SetWindowText(hEdit, "");
	return 0;
}
VOID  WatchChanges(HWND hwnd, BOOL(__stdcall *Action)(HWND))
{
	BOOL DontClose = FALSE;
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		if (!Compare(hEdit))
		{
			switch (MessageBox(hwnd, "Сохранить изменения в файле?", " Вопрос ", MB_YESNOCANCEL | MB_ICONQUESTION))
			{
			case IDYES:
			{
				SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
				//SendMessage(hwnd, WM_DESTROY,0,0);
			}
			//break;
			case IDNO:
			{
				Action(hwnd);
				//SendMessage(hwnd, WM_DESTROY, 0, 0);
			}
			break;
			case IDCANCEL:
			{
				DontClose = TRUE;
				break;
			}

			}
		}
		if (!DontClose) Action(hwnd);
	}

}
VOID DoSelectFont(HWND hwnd);

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
	if (strlen(lpCmdLine))
	{
		int a = 0;
		for (int i = 0; i < strlen(lpCmdLine); i++)
		{						
			if (lpCmdLine[i] != '\"')
			{
				szPath[a] = lpCmdLine[i];
				a++;
			}
		}		

		//MessageBox(NULL, szPath, " Вопрос ", MB_OK | MB_ICONQUESTION);
	}


	HWND hwnd = CreateWindowEx
	(
		WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES,
		SZ_CLASS_NAME,
		"Title of my Window",
		WS_OVERLAPPEDWINDOW,
		0,0,640,480,
		NULL,NULL,hInstance,NULL
	);

	//if(szPath[0]) MessageBox(hwnd, szPath, " Info ", MB_OK | MB_ICONQUESTION);
	
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

			HMENU hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_NEW, "&New");
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, "&Open");
			AppendMenu(hSubMenu, MF_SEPARATOR, 0,0);
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_SAVE, "&Save");
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_SAVEAS, "Sa&ve As");
			AppendMenu(hSubMenu, MF_SEPARATOR, 0,0);
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");
		
			hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu, MF_STRING, ID_FORMAT_FONT, "Fon&t");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "F&ormat");

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

			if (szPath[0])
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
				LoadTextFileToEdit(hEdit, szPath);
			}
			
			//Регестрация горячих клавиш без акселератора(в resourse.h внесены ресурсы 201,202,203)
			RegisterHotKey(hwnd,HOTKEY_NEW,MOD_CONTROL/*Зажатый CTRL*/, 'N'/*Название клавиши в сочетании с CTRL*/);
			RegisterHotKey(hwnd,HOTKEY_OPEN,MOD_CONTROL/*Зажатый CTRL*/, 'O'/*Название клавиши в сочетании с CTRL*/);
			RegisterHotKey(hwnd,HOTKEY_SAVE,MOD_CONTROL/*Зажатый CTRL*/, 'S'/*Название клавиши в сочетании с CTRL*/);
			RegisterHotKey(hwnd, HOTKEY_SAVEAS, MOD_CONTROL + MOD_ALT/*Зажатый CTRL+ALT*/, 'S'/*Название клавиши в сочетании с CTRL+ALT*/);



			// Создание панели управления (в resourse.h внесен ресурс 112)
			HWND hTool = CreateWindowEx
			(
				0,
				TOOLBARCLASSNAME,
				NULL,
				WS_CHILD | WS_VISIBLE,
				0, 0,
				0, 0,
				hwnd,
				(HMENU)IDC_MAIN_TOOL,
				GetModuleHandle(NULL),
				NULL
			);
			SendMessage(hTool, TB_BUTTONSTRUCTSIZE,(WPARAM)sizeof(TBBUTTON),0);
			// Кнопки панели инстр.
			TBBUTTON tbb[3]; //Три кнопки
			TBADDBITMAP tbab;
			tbab.hInst = HINST_COMMCTRL;
			tbab.nID = IDB_STD_SMALL_COLOR;
			SendMessage(hTool, TB_ADDBITMAP, 0, (LPARAM)&tbab);
			ZeroMemory(&tbb,sizeof(tbb));

			tbb[0].iBitmap = STD_FILENEW;
			tbb[0].fsState = TBSTATE_ENABLED;
			tbb[0].fsStyle = TBSTYLE_BUTTON;
			tbb[0].idCommand = ID_FILE_NEW;

			tbb[1].iBitmap = STD_FILEOPEN;
			tbb[1].fsState = TBSTATE_ENABLED;
			tbb[1].fsStyle = TBSTYLE_BUTTON;
			tbb[1].idCommand = ID_FILE_OPEN;

			tbb[2].iBitmap = STD_FILESAVE;
			tbb[2].fsState = TBSTATE_ENABLED;
			tbb[2].fsStyle = TBSTYLE_BUTTON;
			tbb[2].idCommand = ID_FILE_SAVE;

			SendMessage(hTool, TB_ADDBUTTONS, sizeof(tbb)/sizeof(TBBUTTON),(LPARAM)&tbb);



			//////////////////////// StatusBar ////////////////////////////////////////////////
			HWND hStatus = CreateWindowEx
			(
				0,
				STATUSCLASSNAME,
				NULL,
				WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
				0,0,
				0,0,
				hwnd,
				(HMENU)IDC_MAIN_STATUS,
				GetModuleHandle(NULL),
				NULL
			);

			// разделител и status bar
			int statwidth[] = {150, 250, -1};
			SendMessage(hStatus, SB_SETPARTS, sizeof(statwidth)/ sizeof(int), (LPARAM)statwidth);
			/*SendMessage(hStatus, SB_SETTEXT,0, (LPARAM)"This is the status bar...");
			SendMessage(hStatus, SB_SETTEXT,3, (LPARAM)szPath);*/


			// FONT
			HDC hdc = GetDC(NULL);
			int lfHeight = MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72); // Определение высоты шрифта
			ReleaseDC(NULL, hdc);
			HFONT hFont = CreateFont
			(
				lfHeight,0,0,0,0,
				FALSE,
				0,
				0,
				RUSSIAN_CHARSET,
				0,
				0,
				0,
				0,
				"Times New Roman"				
				);
			g_hFont = hFont; 
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, 0);
		}
		

		break;

		case WM_SIZE:
		{
			HWND hTool = GetDlgItem(hwnd, IDC_MAIN_TOOL);
			SendMessage(hTool, TB_AUTOSIZE, 0, 0);
			RECT rcTool;
			GetWindowRect(hTool, &rcTool);
			int iToolHeight = rcTool.bottom - rcTool.top;
			
			

			/*RECT rcClient;
			GetClientRect(hwnd, &rcClient);
			HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
			SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom,SWP_NOZORDER);*/

			HWND hStatus = GetDlgItem(hwnd, IDC_MAIN_STATUS);
			SendMessage(hStatus, WM_SIZE, 0, 0);
			RECT rcStatus;
			GetWindowRect(hStatus,&rcStatus);
			int iStatusHeight = rcStatus.bottom - rcStatus.top;

			RECT rcClient;
			GetClientRect(hwnd, &rcClient);
			HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
			int iEditHeight = rcClient.bottom - iToolHeight - iStatusHeight;
			SetWindowPos(hEdit, NULL, 0, iToolHeight, rcClient.right, iEditHeight, SWP_NOZORDER);
		}
		break;


		//Вхождение в горячие клавиши
		case WM_HOTKEY:
		{
			switch (wParam)
			{
			case HOTKEY_NEW:SendMessage(hwnd, WM_COMMAND, ID_FILE_NEW, 0); break;
			case HOTKEY_OPEN:SendMessage(hwnd,WM_COMMAND,ID_FILE_OPEN,0); break;
			case HOTKEY_SAVE:SendMessage(hwnd,WM_COMMAND,ID_FILE_SAVE,0); break;
			case HOTKEY_SAVEAS:SendMessage(hwnd,WM_COMMAND,ID_FILE_SAVEAS,0); break;
			}
		}
		break;

		case WM_COMMAND:
		{
			switch (LOWORD(wParam)) // LOWORD содержит ID сообщения
			{
				case ID_FORMAT_FONT:
				{
					DoSelectFont(hwnd);
				}
				break;

				case ID_FILE_NEW:
				{					
					WatchChanges(hwnd, DoFileNew);
				}
				break;

				case ID_FILE_OPEN:
				{
					//if (szPath[0] != '\0')
					//{
					//	    //MessageBox(hwnd, szPath, " Вопрос ", MB_OK | MB_ICONINFORMATION);
					//	    HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
					//		LoadTextFileToEdit(hEdit, szPath);							
					//}
					//else
					WatchChanges(hwnd, DoFileOpen);

					////создадим стандартное окно открытия /сохранения файла:
					//OPENFILENAME ofn;  // создаем структуру 
					//CHAR szFileName[MAX_PATH] = {}; 

					//ZeroMemory(&ofn, sizeof(ofn)); // зануление полей структуры

					//// Заполняем поля структуры
					//ofn.lStructSize = sizeof(ofn);
					//ofn.hwndOwner = hwnd; // определитель родительского объекта
					//ofn.lpstrFilter = szFilter; // константа в начале кода
					//ofn.lpstrFile = szFileName; // строка куда сохранится путь к файлу
					//ofn.nMaxFile = MAX_PATH; // Максимально возможная длина пути 256 байт
					//ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY; // флаги открытия файла
					//ofn.lpstrDefExt = "txt"; // разрешение по умолчанию с которым сохраняется файл
			
					//if (GetOpenFileName(&ofn)) // Если получилось открыть файл
					//{
					//	//Происходит чето-там
					//	HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
					//	LoadTextFileToEdit(hEdit, szFileName);
					//	strncpy_s(szPath, szFileName, sizeof(szPath) - 1);				
					//}
				
				}
				break;
				
				case ID_FILE_SAVE:
				{

					DoFileSave(hwnd);
					//if(szPath[0]!='\0')
					//{
					//		//Происходит чето-там
					//		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
					//		SaveTextFileFromEdit(hEdit, szPath);
					//}
					//else
					//{				
					//	SendMessage(hwnd, WM_COMMAND,(WPARAM)ID_FILE_SAVEAS, 0);
					//}

				}
				break;
				case ID_FILE_SAVEAS:
				{
					DoFileSaveAS(hwnd);
					//OPENFILENAME ofn;  // создаем структуру 
					//CHAR szFileName[MAX_PATH] = {};

					//ZeroMemory(&ofn, sizeof(ofn)); // зануление полей структуры

					//// Заполняем поля структуры
					//ofn.lStructSize = sizeof(ofn);
					//ofn.hwndOwner = hwnd; // определитель родительского объекта
					//ofn.lpstrFilter = szFilter; // константа в начале кода
					//ofn.lpstrFile = szFileName; // строка куда сохранится путь к файлу
					//ofn.nMaxFile = MAX_PATH; // Максимально возможная длина пути 256 байт
					//ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY; // флаги открытия файла
					//ofn.lpstrDefExt = "txt"; // разрешение по умолчанию с которым сохраняется файл

					//if (GetSaveFileName(&ofn)) // Если получилось открыть файл
					//{
					//	//Происходит чето-там
					//	HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
					//	SaveTextFileFromEdit(hEdit, szFileName);
					//	strncpy_s(szPath, szFileName, sizeof(szPath) - 1);
					//}
				}
				break;
				case ID_FILE_EXIT:
				{
					SendMessage(hwnd,WM_COMMAND,WM_CLOSE,0);
				}
				break;
				case ID_HELP_ABOUT:
				{			
					DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, DlgProc, 0);			
				}
				break;
				case ID_HELP_USER:
				{			
					switch (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_USER), hwnd, DlgUsrProc))
					{
					 case IDOK: MessageBox(hwnd, "Dialog ended with OK!", "Info", MB_OK | MB_ICONINFORMATION); break;
					 case IDCANCEL: MessageBox(hwnd, "Dialog ended with CANCEL!", "Info", MB_OK | MB_ICONINFORMATION); break;
					 case -1: MessageBox(hwnd, "Dialog eFailed!", "Error", MB_OK | MB_ICONERROR); break;
					}
				}
				break;
				case ID_F1:
				{
					if (szPath[0])
					{
						HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
						LoadTextFileToEdit(hEdit, szPath);
					}
					//MessageBox(hwnd, "Use key F1!", "Info", MB_OK | MB_ICONINFORMATION);
					//DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, DlgProc, 0);
				}
				break;
			}	
		}
		break;
		/*case WM_KEYDOWN:
		{
			switch (wParam)
			{
			default:
				MessageBox(hwnd, "Use key F1!", "Info", MB_OK | MB_ICONINFORMATION);
					break;
			}
			
		}
		break;*/
		case WM_DROPFILES:
		{		
			//Поиск пути к переташеному файлу
			int SizeDragFile = DragQueryFile((HDROP)wParam, 0, NULL, 0);
			char* PachToFile = new char[SizeDragFile + 1];
			DragQueryFile((HDROP)wParam, 0, PachToFile, SizeDragFile + 1);
			PachToFile[SizeDragFile] = '\0';			
			DragFinish((HDROP)wParam);	

			//Открытие файла в редакторе
			strncpy_s(szPath, PachToFile, sizeof(szPath) - 1); 			
			HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
			LoadTextFileToEdit(hEdit, szPath);
		}
		break;
	/*	case WM_LBUTTONDOWN:
			DestroyWindow(hwnd);
			break;*/

		case WM_MOUSEMOVE:			
		{
		int a = 0;
		a++;
		}
			break;
		case WM_CLOSE:
		{
			WatchChanges(hwnd, DestroyWindow);
			//BOOL DontClose = FALSE; 			
			//{
			//	HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
			//	if (!Compare(hEdit))
			//	{

			//		switch (MessageBox(hwnd, "Сохранить изменения в файле?", " Вопрос ", MB_YESNOCANCEL | MB_ICONQUESTION))
			//		{
			//			case IDYES:
			//			{
			//				SendMessage(hwnd,WM_COMMAND, ID_FILE_SAVE,0);
			//				//SendMessage(hwnd, WM_DESTROY,0,0);
			//			}
			//			//break;
			//			case IDNO:
			//			{
			//				SendMessage(hwnd, WM_DESTROY, 0, 0);
			//			}
			//			break;
			//			case IDCANCEL:
			//			{
			//				DontClose = TRUE;
			//				break;
			//			}
			//			
			//		}
			//	}
			//	if(!DontClose) DestroyWindow(hwnd);
			//}
		}
			break;
		case WM_DESTROY:
			
		{		
			GlobalFree(lpszCurrentText);
			PostQuitMessage(0);
		}
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
		/*case WM_KEYDOWN:
		{
			switch (wParam)
			{
			default:
				MessageBox(hwnd, "Use key F1!", "Info", MB_OK | MB_ICONINFORMATION);
				break;
			}
		}*/
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
				
				}
				break;

				case IDCANCEL:
				{
					EndDialog(hwnd, IDCANCEL);					
				}
				break;
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
//	MessageBox(hEdit, "1", "Info", MB_OK | MB_ICONINFORMATION);
	BOOL bSuccess = FALSE; // флаг успешности
	//MessageBox(hEdit, pszFileName, "Info", MB_OK | MB_ICONINFORMATION);	
	HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL); // Обработчик файла
	if (hFile != INVALID_HANDLE_VALUE)
	{
	//	MessageBox(hEdit, "2", "Info", MB_OK | MB_ICONINFORMATION);
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0xFFFFFFFF) // проверяем не привышает ли максимальный размер
		{
			
			//MessageBox(hEdit, "3", "Info", MB_OK | MB_ICONINFORMATION);
			LPSTR pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1); // Выделяем память для чтения файла
			
			if (lpszCurrentText) GlobalFree(lpszCurrentText);
			lpszCurrentText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1); 

			if (pszFileText)
			{
				//MessageBox(hEdit, "4", "Info", MB_OK | MB_ICONINFORMATION);
				
				DWORD dwRead;
				if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{		
					//MessageBox(hEdit, "5", "Info", MB_OK | MB_ICONINFORMATION);
					strcpy_s(lpszCurrentText, dwFileSize+1,pszFileText);
					pszFileText[dwFileSize] = 0; // Ставим в конец 0
					if (SetWindowText(hEdit, pszFileText))
					{
						
						bSuccess = TRUE;
						//Передача  названия открытого файла в заголовок основного окна
						SetWindowsTitle(hEdit);
						HWND hStatus = GetDlgItem(GetParent(hEdit), IDC_MAIN_STATUS);
						
						//unsigned char bytes[sizeof(DWORD)];
						//memcpy(bytes, &dwFileSize, sizeof bytes);
						sprintf_s(szSize, 256, "%d", dwFileSize);
						
						int a = strlen(szSize);						
						szSize[a] = 'B';
						szSize[a +1] = '\0';

						SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"This is the status bar...");
						SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)szSize);
						SendMessage(hStatus, SB_SETTEXT, 2, (LPARAM)szPath);
					}
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}
/*******************************************     SaveTextFileFromEdit   ****************************************************************/
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR pszFileName)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength = GetWindowTextLength(hEdit);
		if (dwTextLength > 0)
		{

			if (lpszCurrentText) GlobalFree(lpszCurrentText);
			lpszCurrentText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
			GetWindowText(hEdit, lpszCurrentText, dwTextLength + 1);

			LPSTR pszEditText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
			if (pszEditText != NULL)
			{
				if (GetWindowText(hEdit, pszEditText, dwTextLength + 1))
				{
					DWORD dwWritten;
					if (WriteFile(hFile, pszEditText, dwTextLength, &dwWritten, NULL)) bSuccess = TRUE;
					strcpy_s(lpszCurrentText, dwTextLength + 1, pszEditText);
					SetWindowsTitle(hEdit);
				}
				GlobalFree(pszEditText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}
/*******************************************   Compare   ******************************************************************************/
BOOL Compare(HWND hEdit)
{
	// Сранение последнего сохраненого текста с написаным
	BOOL boolanswer = TRUE;
	DWORD dwTextLength = GetWindowTextLength(hEdit);
	LPSTR lpszEditText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
//	DWORD dwFileTextLendth = szPath ? strlen(szPath) : 0;
		
		if (lpszCurrentText!= NULL)
		{
			if (GetWindowText(hEdit, lpszEditText, dwTextLength + 1))
			{							
				if (strcmp(lpszCurrentText,lpszEditText)) boolanswer = FALSE;				
			}			
		}
		else
		{
			if (dwTextLength > 0) // Если поле текста не пусто но файл не подгружался все равно предлагаем сохранение
			{
				boolanswer = FALSE;
			}
		}
		GlobalFree(lpszEditText);
	
	return boolanswer;
}
/*******************************************   DoFileSaveAS   ******************************************************************************/
VOID DoFileSaveAS(HWND hwnd)
{
	OPENFILENAME ofn;  // создаем структуру 
	//CHAR szFileName[MAX_PATH] = {};

	ZeroMemory(&ofn, sizeof(ofn)); // зануление полей структуры

	// Заполняем поля структуры
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd; // определитель родительского объекта
	ofn.lpstrFilter = szFilter; // константа в начале кода
	ofn.lpstrFile = szPath; // строка куда сохранится путь к файлу
	ofn.nMaxFile = MAX_PATH; // Максимально возможная длина пути 256 байт
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY; // флаги открытия файла
	ofn.lpstrDefExt = "txt"; // разрешение по умолчанию с которым сохраняется файл

	if (GetSaveFileName(&ofn)) // Если получилось открыть файл
	{
		//Происходит чето-там
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SaveTextFileFromEdit(hEdit, szPath);

		//strncpy_s(szPath, szFileName, sizeof(szPath) - 1);
	}
}
/*******************************************   DoFileOpen   ******************************************************************************/
BOOL __stdcall DoFileOpen(HWND hwnd)
{
	//создадим стандартное окно открытия /сохранения файла:
	OPENFILENAME ofn;  // создаем структуру 
	//CHAR szFileName[MAX_PATH] = {};

	ZeroMemory(&ofn, sizeof(ofn)); // зануление полей структуры

	// Заполняем поля структуры
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd; // определитель родительского объекта
	ofn.lpstrFilter = szFilter; // константа в начале кода
	ofn.lpstrFile = szPath;//szFileName; // строка куда сохранится путь к файлу
	ofn.nMaxFile = MAX_PATH; // Максимально возможная длина пути 256 байт
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY; // флаги открытия файла
	ofn.lpstrDefExt = "txt"; // разрешение по умолчанию с которым сохраняется файл

	if (GetOpenFileName(&ofn)) // Если получилось открыть файл
	{
		//Происходит чето-там
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		LoadTextFileToEdit(hEdit, szPath/*szFileName*/);
		//MessageBox(hwnd, szPath, " Вопрос ", MB_YESNOCANCEL | MB_ICONQUESTION);
		//strncpy_s(szPath, szFileName, sizeof(szPath) - 1);
		return TRUE;
	}
	return FALSE;
}
/*******************************************   DoFileSave  ******************************************************************************/
BOOL __stdcall DoFileSave(HWND hwnd)
{
	if (szPath[0] != '\0')
	{
		//Происходит чето-там
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SaveTextFileFromEdit(hEdit, szPath);
	}
	else
	{
		SendMessage(hwnd, WM_COMMAND, (WPARAM)ID_FILE_SAVEAS, 0);
	}
	return 0;

}
/*******************************************   SetWindowsTitle  ******************************************************************************/
VOID SetWindowsTitle(HWND hEdit)
{
	HWND hwndParent = GetParent(hEdit);
	CHAR szTitle[MAX_PATH] = "SimpleWindowEdit - ";
	LPSTR lpszNameOnly = strrchr(szPath, '\\') + 1;
	strcat_s(szTitle, MAX_PATH, lpszNameOnly);
	SetWindowText(hwndParent, szTitle);
}
/*******************************************   DoSelectFont  ******************************************************************************/
VOID DoSelectFont(HWND hwnd)
{
	CHOOSEFONT cf;
	LOGFONT lf;

	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	//ZeroMemory(&lf, sizeof(lf));

	GetObject(g_hFont, sizeof(LOGFONT), &lf);

	cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hwnd;
	cf.lpLogFont = &lf;
	cf.rgbColors = g_rgbText;
	if (ChooseFont(&cf))
	{
		HFONT hf = CreateFontIndirect(&lf);
		if (hf)
		{
			g_hFont = hf;
			
		}
		else
		{
			MessageBox(hwnd, "Font creation is failed", "Error", MB_OK | MB_ICONERROR);

		}
		g_rgbText = cf.rgbColors;
		SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), WM_SETFONT, (WPARAM)g_hFont, TRUE);		
	}

}
