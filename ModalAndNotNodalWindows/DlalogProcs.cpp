#include"DialogProcs.h"


HINSTANCE hInst; // Variable
HWND hwndNotModal;

/***********************************DlgMain**************************************************/
BOOL CALLBACK DlgMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hInst = GetModuleHandle(NULL);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_BTN_MODAL:
		{
			DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_MODAL), hwnd, DlgModal, 0);
			break;
		}
		case ID_BTN_NOT_MODAL:
		{
			hwndNotModal = CreateDialog(hInst, MAKEINTRESOURCE(IDD_NMODAL), 0, DlgNotModal, 0 );
			ShowWindow(hwndNotModal, SW_SHOW);
			//DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_NMODAL), 0, DlgNotModal, 0);
			break;
		}
		}
		break;
	}
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return FALSE;
		break;
	}
	return FALSE;

	}
	return FALSE;
}
/**********************************DlgModal***************************************************/
BOOL CALLBACK DlgModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg)
	{
	case WM_INITDIALOG:
	{

		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			MessageBox(hwnd, "���� �������?", "Question", MB_OK | MB_ICONQUESTION);
			break;
		}
		case IDCANCEL:
		{
			EndDialog(hwnd, 0);
			return TRUE;
			break;
		}
		}
		break;

	}
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return FALSE;
		break;
	}
	return FALSE;

	}
	return FALSE;
}
/**********************************DlgNotModal***************************************************/
BOOL CALLBACK DlgNotModal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg)
	{
	case WM_INITDIALOG:
	{

		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			MessageBox(hwnd, "���� �������?", "Question", MB_OK | MB_ICONQUESTION);
			break;
		}
		case IDCANCEL:
		{
			EndDialog(hwnd, 0);
			return TRUE;
			break;
		}
		}

		break;
	}
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return FALSE;
		break;
	}
	return FALSE;

	}
	return FALSE;
}