#include "FrameLimiter.h"
#include "Administrator.h"
#include "Window.h"
#include "DeviceContext.h"
#include "Gui.h"

#include "../external/imgui/imgui.h"

void Main()
{
	gl::FrameLimiter lFrameLimiter(0);
	
	gl::Window* lWindow = gl::Administrator::Get<gl::Window>();
	HWND lHwnd = lWindow->GetHwnd();

	gl::DeviceContext* lDeviceContext = gl::Administrator::Get<gl::DeviceContext>();
	constexpr float kClearColor[] = { 0,0,0,1 };

	gl::Gui* lGui = gl::Administrator::Get<gl::Gui>();

	while (lWindow->Update())
	{
		lFrameLimiter.Update();
		lFrameLimiter.Show(lHwnd);

		lGui->NewFrame();

		lDeviceContext->Reset();
		lDeviceContext->SetBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		lDeviceContext->SetViewport(gl::Window::kSize);
		lDeviceContext->ClearScreen(kClearColor);

		lGui->Render();

		lDeviceContext->SetBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		lDeviceContext->Execute();

		lGui->WindowUpdate();

		lDeviceContext->Present();
		lDeviceContext->WaitGpu();
	}

	lDeviceContext->WaitGpu();
}