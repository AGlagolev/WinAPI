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
	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)lParam == GetDlgItem(hwnd, IDC_STATIC1))
		{
			HDC hdc = (HDC)wParam;
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 200));
			return (int)GetStockObject(NULL_BRUSH);
			//break;
		}
	}
	break;
	case WM_CTLCOLOREDIT:
	{
		
			HDC hdc = (HDC)wParam;
			SetBkMode(hdc, OPAQUE);
			SetTextColor(hdc, RGB(255, 255, 255));
			HBRUSH hBrushColor = CreateSolidBrush(RGB(255, 255, 255));
			SetTextColor(hdc, RGB(200,0,0));
			return (LRESULT)hBrushColor;
			//break;
		
	}
	break;
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
			MessageBox(hwnd, "Кого послать?", "Question", MB_OK | MB_ICONQUESTION);
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
			MessageBox(hwnd, "Кого послать?", "Question", MB_OK | MB_ICONQUESTION);
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