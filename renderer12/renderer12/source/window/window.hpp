#pragma once
#include <Windows.h>

namespace re12::detail
{
	class Window
	{
	private:
		// タイトルバー表示内容
		static constexpr const char* kTitle = "Renderer12";

		// exe起動中におけるカーソル表示の有無
		static constexpr bool kShowCursor = true;
	private:
		HWND mHwnd = {};
	public:
		// ウィンドウサイズ格納変数
		struct
		{
			UINT mWidth = 0;
			UINT mHeight = 0;
		} mSize;
	public:
		Window() = default;
		~Window() = default;
	public:
		// 初期化
		void Initialize(UINT _Width, UINT _Height);
		
		// ゲームループ更新
		bool Update();

		// ウィンドウ表示
		void Show(int _CmdShow);
	public:
		HWND GetHwnd()noexcept { return mHwnd; }
	};
}