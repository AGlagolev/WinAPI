//#include <Windows.h>
//#include"resource.h"
#include"DialogProcs.h"


/***********************************WinMain**************************************************/
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), 0, DlgMain, 0);

	return 0;
}

