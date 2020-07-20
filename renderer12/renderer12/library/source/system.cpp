#include "system.h"
#include "d3d12_core.h"
#include "utility.h"

namespace snd::detail
{
	Framework::~Framework()
	{
		utility::SafeDelete<D3D12Core>(d3d12_core_);
	}

	void Framework::Initialize(HWND _hwnd)
	{
		hwnd_ = _hwnd;

		d3d12_core_ = new D3D12Core();
		d3d12_core_->Initialize(hwnd_);

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