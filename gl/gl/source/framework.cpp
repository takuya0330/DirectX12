#include "framework.h"
#include "framerate.h"
#include "administrator.h"
#include "device.h"
#include "device_context.h"

namespace
{
	constexpr const char* kWindowTitle = "gl";
	constexpr bool kShowCursor = true;

	HWND setup_window(WNDPROC _wndproc, HINSTANCE _hinstance, UINT _width, UINT _height)
	{
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = _wndproc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = _hinstance;
		wcex.hIcon = 0;// LoadIcon(wcex.hInstance, reinterpret_cast<LPCWSTR>(IDI_ICON1));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"gl";
		wcex.hIconSm = 0;// LoadIcon(wcex.hInstance, reinterpret_cast<LPCWSTR>(IDI_ICON1));
		RegisterClassEx(&wcex);
		RECT rect = { 0, 0, (LONG)_width, (LONG)_height };
		DWORD style = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
		AdjustWindowRect(&rect, style, false);

		return CreateWindowA("gl", kWindowTitle, style, CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(rect.right - rect.left), static_cast<int>(rect.bottom - rect.top), nullptr, nullptr, _hinstance, nullptr);
	}

	bool update()
	{
		MSG msg = {};
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
}

int framework::run(WNDPROC _wndproc, HINSTANCE _hinstance, int _cmdshow)
{
	HWND hwnd = setup_window(_wndproc, _hinstance, gl::kWindowSize.x, gl::kWindowSize.y);

	ShowWindow(hwnd, _cmdshow);
	ShowCursor(kShowCursor);

	//CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	gl::framerate fps(120);
	gl::administrator admin(hwnd);

	gl::device_context* render_command = gl::administrator::get<gl::device_context>();
	constexpr float kClearColor[] = { 1.0f,0.7f,0.0f,1 };

	while (update())
	{
		fps.run();
		fps.show(hwnd);

		render_command->reset();
		render_command->barrier_transition(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		render_command->set_viewport(gl::kWindowSize);
		render_command->clear(kClearColor);
		render_command->barrier_transition(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		render_command->present();
		render_command->wait_previous_frame();
	}

	//CoUninitialize();

	return 0;
}
