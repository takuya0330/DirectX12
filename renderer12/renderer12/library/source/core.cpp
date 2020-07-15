#include "core.h"
#include "device.h"
#include "renderer.h"
#include "utility.h"

namespace snd::detail
{
	System::~System()
	{
		utility::SafeDelete<Renderer>(renderer_);
		utility::SafeDelete<Device>(device_);
	}

	void System::Initialize(HWND _hwnd)
	{
		hwnd_ = _hwnd;

		device_ = new Device();
		device_->Create();

		renderer_ = new Renderer();
		renderer_->Create(hwnd_, device_->Get(), device_->GetFactory());
	}

	bool System::Update()
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