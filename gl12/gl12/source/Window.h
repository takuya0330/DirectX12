#pragma once
#include <Windows.h>

#include "Mathematics.h"

namespace gl
{
	class Window
	{
		static constexpr const char* kTitle = "Graphics Library";
		static constexpr bool kShowCursor = true;
	private:
		HWND mHwnd;
	public:
		static constexpr uint2 kSize = uint2(1280, 720);
	public:
		Window() = default;
		~Window() = default;
	public:
		void Initialize();
		bool Update();
		void Show(int _CmdShow);
	public:
		HWND GetHwnd()noexcept { return mHwnd; }
	};
}