#include "Window.h"

namespace gl
{
	void Window::Initialize()
	{
		RECT lRect = { 0, 0, kSize.x, kSize.y };
		DWORD lStyle = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
		AdjustWindowRect(&lRect, lStyle, false);

		mHwnd = CreateWindowA("Title", kTitle, lStyle, CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(lRect.right - lRect.left), static_cast<int>(lRect.bottom - lRect.top), nullptr, nullptr, GetModuleHandle(NULL), nullptr);
	}

	bool Window::Update()
	{
		MSG lMsg{};
		while (WM_QUIT != lMsg.message)
		{
			if (PeekMessage(&lMsg, NULL, 0, 0, PM_REMOVE)) { TranslateMessage(&lMsg); DispatchMessage(&lMsg); }
			else { return true; }
		}
		return false;
	}

	void Window::Show(int _CmdShow)
	{
		ShowWindow(mHwnd, _CmdShow);
		ShowCursor(kShowCursor);
	}
}