#include <Windows.h>
#include <crtdbg.h>
#include <time.h>
#include <locale>

#include "../resource/resource.h"
#include "core.h"

namespace
{
	constexpr const char* kWindowTitle = "snd";
	constexpr bool kShowCursor = true;

	constexpr UINT kWindowWidth = 1600;
	constexpr UINT kWindowHeight = 900;
}

extern void Main();

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); break;
		case VK_F11: break;
		}
	}
	break;
	default: return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int CmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	srand(static_cast<u_int>(time(nullptr)));
	setlocale(LC_CTYPE, "jpn");
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(wcex.hInstance, reinterpret_cast<LPCWSTR>(IDI_ICON1));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"snd";
		wcex.hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<LPCWSTR>(IDI_ICON1));
	}
	RegisterClassEx(&wcex);
	RECT rect = { 0, 0, kWindowWidth, kWindowHeight };
	DWORD style = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
	AdjustWindowRect(&rect, style, false);

	HWND hwnd = CreateWindowA
	(
		"snd",
		kWindowTitle,
		style,
		0,
		0,
		static_cast<int>(rect.right - rect.left),
		static_cast<int>(rect.bottom - rect.top),
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	ShowWindow(hwnd, CmdShow);
	ShowCursor(kShowCursor);

	snd::Core::Create();
	snd::Core::Ref().Initialize(hwnd);

	Main();

	snd::Core::Destroy();

	return 0;
}