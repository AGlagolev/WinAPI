#include<Windows.h>

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	if(MessageBox(NULL, "Hello WinAPI ;-) - ��� ���������", "Header - ��� ��������� ����", MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2 | MB_SYSTEMMODAL) == IDOK)
	{
		MessageBox(NULL, "�� ������ OK", "���������", MB_OK | MB_ICONINFORMATION);
	} else MessageBox(NULL, "�� ������ Cancel", "���������", MB_OK | MB_ICONINFORMATION);
	
	
	// MB_DEFBUTTON2 - ���������� ����� ������ �������� MB_SYSTEMMODAL - ������ ���� ��������� (������ ���� ���������� ������������)


}