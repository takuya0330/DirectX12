#include <Windows.h>
#include <crtdbg.h>
#include <time.h>
#include <locale>

#include "MsgProc.h"
#include "Administrator.h"
#include "Window.h"

void Main();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int CmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	srand(static_cast<u_int>(time(nullptr)));
	setlocale(LC_CTYPE, "jpn");

	// ToDo. ÉAÉCÉRÉìê›íË
	WNDCLASSEX lWCEX;
	{
		lWCEX.cbSize = sizeof(WNDCLASSEX);
		lWCEX.style = CS_HREDRAW | CS_VREDRAW;
		lWCEX.lpfnWndProc = MsgProc;
		lWCEX.cbClsExtra = 0;
		lWCEX.cbWndExtra = 0;
		lWCEX.hInstance = hInstance;
		lWCEX.hIcon = 0;// LoadIcon(lWndClassEX.hInstance, reinterpret_cast<LPCWSTR>(IDI_ICON1));
		lWCEX.hCursor = LoadCursor(NULL, IDC_ARROW);
		lWCEX.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		lWCEX.lpszMenuName = nullptr;
		lWCEX.lpszClassName = L"Title";
		lWCEX.hIconSm = 0;// LoadIcon(lWndClassEX.hInstance, reinterpret_cast<LPCWSTR>(IDI_ICON1));
		RegisterClassEx(&lWCEX);
	}

	gl::Administrator lAdministrator;
	gl::Administrator::Get<gl::Window>()->Show(CmdShow);

	Main();

	return 0;
}