#include "source/system.h"
#include "source/graphics_core.h"
#include "source/frame_rate.h"

void Main()
{
	constexpr float clear_color[] = { 0,0,0,1 };

	const HWND hwnd = snd::System::Ref().GetHWND();

	snd::GraphicsCore* graphics = snd::System::Ref().GetGraphicsCore();

	snd::FrameRate frame_rate(0);

	while (snd::System::Ref().Update())
	{
		frame_rate.Run();
		frame_rate.ShowFPS(hwnd);

		graphics->Reset();
		graphics->Barrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		
		graphics->SetViewport(snd::constants::kResolution);
		graphics->Clear(clear_color);

		graphics->Barrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		graphics->Present();
		graphics->WaitForCommandQueue();
	}
}