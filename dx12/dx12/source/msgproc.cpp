#include <Windows.h>

LRESULT CALLBACK MsgProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
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
		case VK_F11: break;
		}
	}
	break;
	default: return DefWindowProc(_hwnd, _msg, _wparam, _lparam);
	}
	return 0;
}