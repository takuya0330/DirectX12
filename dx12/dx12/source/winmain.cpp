#include <Windows.h>
#include <crtdbg.h>
#include <time.h>
#include <locale>

#include "../header/engine.h"
#include "../header/window.h"

LRESULT CALLBACK MsgProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam);

void Main();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int CmdShow)
{
	// �������[���[�N���o
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	// ���Ԃŗ�����ݒ�
	srand(static_cast<u_int>(time(nullptr)));
	// ���{��ɐݒ�
	setlocale(LC_CTYPE, "jpn");

	// �E�B���h�E�N���X�̓o�^
	WNDCLASSEX wcex =
	{
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = MsgProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = 0,
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		.lpszMenuName = nullptr,
		.lpszClassName = L"Title",
		.hIconSm = 0
	};
	RegisterClassEx(&wcex);

	Engine engine;
	Engine::Get<Window>()->Show(CmdShow);

	Main();

	return 0;
}