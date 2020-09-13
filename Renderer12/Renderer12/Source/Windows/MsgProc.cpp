#include "MsgProc.h"

#include "../Administrator.h"
#include "../Input/Mouse.h"

//#include "../ThirdParty/imgui/imgui.h"
//#include "../ThirdParty/imgui/imgui_impl_win32.h"
//#include "../ThirdParty/imgui/imgui_impl_dx12.h"

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MsgProc(HWND _hWnd, UINT _Msg, WPARAM _wParam, LPARAM _lParam)
{
	//if (ImGui_ImplWin32_WndProcHandler(_hWnd, _Msg, _wParam, _lParam)) return true;

	PAINTSTRUCT lPaintStruct;
	HDC lHdc;
	static POINTS lCursor;

	switch (_Msg)
	{
	case WM_PAINT:
		lHdc = BeginPaint(_hWnd, &lPaintStruct);
		EndPaint(_hWnd, &lPaintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (_wParam)
		{
		case VK_ESCAPE:
			PostMessage(_hWnd, WM_CLOSE, 0, 0);
			break;
		case VK_F2:
			// ToDo. ウィンドウモード切替
			break;
		case VK_F11:
			// ToDo. スクリーンショット
			break;
		}
		break;
	case WM_LBUTTONDOWN: case WM_RBUTTONDOWN:
		lCursor = MAKEPOINTS(_lParam);
		SetCapture(_hWnd);
		break;
	case WM_LBUTTONUP: case WM_RBUTTONUP:
		ReleaseCapture(); 
		break;
	case WM_MOUSEMOVE:
		if (gl::Mouse* lMouse = Administrator::Get<gl::Mouse>(); 
			lMouse->State(gl::Buttons::eMouseLeft, gl::Trigger::eKeep) || lMouse->State(gl::Buttons::eMouseRight, gl::Trigger::eKeep))
		{
			POINTS lPoint = MAKEPOINTS(_lParam);
			lMouse->SetDrag(int2(lPoint.x - lCursor.x, lPoint.y - lCursor.y));
		}
		else
		{
			lMouse->SetDrag(0);
		}
		break;
	case WM_MOUSEWHEEL:
		Administrator::Get<gl::Mouse>()->AddScroll(GET_WHEEL_DELTA_WPARAM(_wParam) / WHEEL_DELTA);
		break;
	default:
		return DefWindowProc(_hWnd, _Msg, _wParam, _lParam);
	}
	return 0;
}