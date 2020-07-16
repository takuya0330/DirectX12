#include "system.h"
#include "graphics_core.h"
#include "utility.h"

namespace snd::detail
{
	Framework::~Framework()
	{
		utility::SafeDelete<GraphicsCore>(graphics_core_);
	}

	void Framework::Initialize(HWND _hwnd)
	{
		hwnd_ = _hwnd;

		graphics_core_ = new GraphicsCore();
		graphics_core_->Initialize(hwnd_);

	}

	bool Framework::Update()
	{
		MSG msg = {};
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessage(&msg); }
			else return true;
		}
		return false;
	}
}