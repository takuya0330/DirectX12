#pragma once
#include <Windows.h>

#include "../Mathematics.h"

enum class ScreenResolution : uint
{
	e720p,
	e900p,
	e1080p,

	eMax
};

namespace gl
{
	class Window
	{
		static constexpr const char* kTitle = "Graphics Library";
		static constexpr bool kShowCursor = true;
		static constexpr uint2 kResolution[static_cast<uint>(ScreenResolution::eMax)] = { uint2(1280,720), uint2(1600,900), uint2(1920,1080) };
	private:
		HWND mHwnd;
		uint mWidth;
		uint mHeight;
	public:
		Window() = default;
		~Window() = default;
	public:
		void Initialize();
		bool Update();
		void Show(int _CmdShow);
	public:
		HWND GetHwnd()noexcept { return mHwnd; }
		uint GetWidth()const noexcept { return mWidth; }
		uint GetHeight()const noexcept { return mHeight; }
	};
}