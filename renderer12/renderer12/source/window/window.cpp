#include "window.hpp"

namespace re12::detail
{
	void Window::Initialize(UINT _Width, UINT _Height)
	{
		mSize.mWidth = _Width;
		mSize.mHeight = _Height;
		RECT lRect = { 0, 0, static_cast<LONG>(mSize.mWidth), static_cast<LONG>(mSize.mHeight) };
		DWORD lStyle = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
		AdjustWindowRect(&lRect, lStyle, false);

		mHwnd = CreateWindowA("Title", kTitle, lStyle, CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(lRect.right - lRect.left), static_cast<int>(lRect.bottom - lRect.top), nullptr, nullptr, GetModuleHandle(NULL), nullptr);
	}

	bool Window::Update()
	{
		MSG lMsg{};
		while (WM_QUIT != lMsg.message)
		{
			if (PeekMessage(&lMsg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&lMsg);
				DispatchMessage(&lMsg);
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	void Window::Show(int _CmdShow)
	{
		ShowWindow(mHwnd, _CmdShow);
		ShowCursor(kShowCursor);
	}
}