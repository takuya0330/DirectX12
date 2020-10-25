#include "administrator/administrator.hpp"
#include "utility/utility.h"
#include "utility/frame_limiter.hpp"
#include "window/window.hpp"
#include "device_context/device_context.hpp"

#include "math/mathmatics.hpp"

#include "../third_party/imgui/imgui.h"

void Main()
{
	constexpr float kClearColor[] = { 0,0,0,1 };
	re12::detail::FrameLimiter lFrameLimiter(0);

	re12::detail::Window* lWindow = re12::Administrator::Get<re12::detail::Window>();
	HWND lHwnd = lWindow->GetHwnd();
	const uint2 lWindowSize = { lWindow->mSize.mWidth, lWindow->mSize.mHeight };

	re12::detail::DeviceContext* lDeviceContext = re12::Administrator::Get<re12::detail::DeviceContext>();


	RE12_OUTPUT_CONSOLE("GameLoop start.");

	while (lWindow->Update())
	{
		lFrameLimiter.Update();
		//lFrameLimiter.Show(lHwnd);

		lDeviceContext->ImGuiUpdate();

		// Application Update
		{
			lDeviceContext->ImGuiFunction("Info", [&]() {
				ImGui::Text("FPS : %d / FrameTime(ms) : %f", lFrameLimiter.GetFPS(), lFrameLimiter.GetElapsedTime());
				});
		}

		lDeviceContext->Reset();
		lDeviceContext->BarrierTransition(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		lDeviceContext->SetViewport(lWindowSize.x, lWindowSize.y);
		lDeviceContext->ClearScreen(kClearColor);

		// Application Render
		{

		}

		lDeviceContext->ImGuiRender();
		lDeviceContext->BarrierTransition(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		lDeviceContext->Execute();
		lDeviceContext->ImGuiConfig();
		lDeviceContext->Present();
		lDeviceContext->WaitFence();
	}
	lDeviceContext->WaitFence();

	RE12_OUTPUT_CONSOLE("GameLoop end.");
}