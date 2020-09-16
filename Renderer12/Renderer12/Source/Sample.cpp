#include "Administrator.h"
#include "Windows/Window.h"
#include "Core/DeviceContext.h"
#include "Input/Mouse.h"
#include "Misc/FrameLimiter.h"

#include "../ThirdParty/imgui/imgui.h"

void Main()
{
	gl::Window* lWindow = Administrator::Get<gl::Window>();
	HWND lHwnd = lWindow->GetHwnd();
	const uint2 lWindowSize = Administrator::Get<gl::Window>()->GetResolution();

	constexpr float kClearColor[] = { 0,0,0,1 };
	gl::DeviceContext* lDeviceContext = Administrator::Get<gl::DeviceContext>();

	gl::Mouse* lMouse = Administrator::Get<gl::Mouse>();

	FrameLimiter lFrameLimiter(120);

	while (lWindow->Update())
	{
		lFrameLimiter.Update();
		lFrameLimiter.Show(lHwnd);

		lDeviceContext->NewFrame();

		{
			lMouse->Update();

			ImGui::Begin("Mouse");
			ImGui::Text("px : %d, py : %d", lMouse->GetPos().x, lMouse->GetPos().y);
			ImGui::Text("dx : %d, dy : %d", lMouse->GetDrag().x, lMouse->GetDrag().y);
			ImGui::End();

			ImGui::Begin("Moji");
			ImGui::Text("aaaaa");
			ImGui::End();
		}

		lDeviceContext->Reset();
		lDeviceContext->BarrierTransition(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		lDeviceContext->SetViewport(lWindowSize);
		lDeviceContext->ClearScreen(kClearColor);

		{

		}

		lDeviceContext->GUIRender();
		lDeviceContext->BarrierTransition(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		lDeviceContext->Execute();
		lDeviceContext->GUIConfig();
		lDeviceContext->Present();
		lDeviceContext->WaitFence();
	}
	lDeviceContext->WaitFence();
}