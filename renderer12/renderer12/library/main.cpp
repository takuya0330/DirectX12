#include "source/core.h"
#include "source/device.h"
#include "source/renderer.h"
#include "source/frame_rate.h"

void Main()
{
	constexpr float clear_color[] = { 0,0,0,1 };

	const HWND hwnd = snd::Core::Ref().GetHWND();
	snd::Device* device = snd::Core::Ref().GetDevice();
	snd::Renderer* renderer = snd::Core::Ref().GetRenderer();

	snd::FrameRate frame_rate(0);

	while (snd::Core::Ref().Update())
	{
		frame_rate.Run();
		frame_rate.ShowFPS(hwnd);

		renderer->Reset();
		renderer->Barrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		renderer->Clear(clear_color);
		renderer->SetViewport(snd::constants::kResolution);

		renderer->Barrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		renderer->Present(0);
		renderer->WaitGPU();
	}
}