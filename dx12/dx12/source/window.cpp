#include "../header/window.h"
#include "../header/constants.h"

void Window::Initialize()
{
	RECT rect = { 0, 0, kWindowWidth, kWindowHeight };
	DWORD style = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
	AdjustWindowRect(&rect, style, false);

	hwnd = CreateWindowA("Title", kWindowTitle, style, CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(rect.right - rect.left), static_cast<int>(rect.bottom - rect.top), nullptr, nullptr, GetModuleHandle(NULL), nullptr);
}

bool Window::Update()
{
	MSG msg{};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			return true;
		}
	}
	return false;
}

void Window::Show(int _cmdshow)
{
	ShowWindow(hwnd, _cmdshow);
	ShowCursor(kShowCursor);
}