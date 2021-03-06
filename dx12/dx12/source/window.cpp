#include "../include/window.h"

void Window::Initialize(const char* _title, UINT _width, UINT _height)
{
	width_ = _width, height_ = _height;
	RECT rect = { 0, 0, static_cast<LONG>(width_), static_cast<LONG>(height_) };
	DWORD style = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
	AdjustWindowRect(&rect, style, false);

	hwnd_ = CreateWindowA(
		"Title",
		_title,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<int>(rect.right - rect.left),
		static_cast<int>(rect.bottom - rect.top),
		nullptr,
		nullptr,
		GetModuleHandle(NULL),
		nullptr);

	ShowWindow(hwnd_, SW_SHOW);
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