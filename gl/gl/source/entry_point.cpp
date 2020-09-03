#include <Windows.h>
#include <crtdbg.h>
#include <time.h>
#include <locale>

#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/imgui_impl_win32.h"
#include "../third_party/imgui/imgui_impl_dx12.h"

#include "framework.h"
#include "administrator.h"
#include "mouse.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wparam, _lparam))
		return true;

	static POINTS m;

	switch (_msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(_hwnd, &ps);
		EndPaint(_hwnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	case WM_KEYDOWN:
	{
		switch (_wparam)
		{
		case VK_ESCAPE: PostMessage(_hwnd, WM_CLOSE, 0, 0); break;
		case VK_F2: break;
		case VK_F11: break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		m = MAKEPOINTS(_lparam);
		SetCapture(_hwnd);
	}
	break;
	case WM_LBUTTONUP:
	{
		ReleaseCapture();
	}
	break;
	case WM_RBUTTONDOWN:
	{
		m = MAKEPOINTS(_lparam);
		SetCapture(_hwnd);
	}
	break;
	case WM_RBUTTONUP:
	{
		ReleaseCapture();
	}
	break;
	case WM_MOUSEMOVE:
	{
		gl::mouse* mouse = gl::administrator::get<gl::mouse>();
		if (mouse->state(gl::buttons::eMouseLeft, gl::trigger::eKeep) || mouse->state(gl::buttons::eMouseRight, gl::trigger::eKeep))
		{
			POINTS p = MAKEPOINTS(_lparam);
			mouse->set_drag(gl::int2(p.x - m.x, p.y - m.y));
		}
		else
		{
			mouse->set_drag(0);
		}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		gl::administrator::get<gl::mouse>()->add_scroll(GET_WHEEL_DELTA_WPARAM(_wparam) / WHEEL_DELTA);
	}
	break;
	default: return DefWindowProc(_hwnd, _msg, _wparam, _lparam);
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

	framework framework;
	return framework.run(WndProc, hInstance, CmdShow);
}
