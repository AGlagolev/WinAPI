#include<Windows.h>

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	if(MessageBox(NULL, "Hello WinAPI ;-) - это сообщение", "Header - это заголовок окно", MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2 | MB_SYSTEMMODAL) == IDOK)
	{
		MessageBox(NULL, "Вы нажали OK", "Заголовок", MB_OK | MB_ICONINFORMATION);
	} else MessageBox(NULL, "Вы нажали Cancel", "Заголовок", MB_OK | MB_ICONINFORMATION);
	
	
	// MB_DEFBUTTON2 - определяет какая кнопка активная MB_SYSTEMMODAL - делает окно модальным (другие окна становятся недоступными)


}