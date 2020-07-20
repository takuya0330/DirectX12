#include "source/system.h"
#include "source/d3d12_core.h"
#include "source/frame_rate.h"

void Main()
{
	constexpr float clear_color[] = { 0,0,0,1 };

	const HWND hwnd = snd::System::Ref().GetHWND();

	snd::D3D12Core* core = snd::System::Ref().GetGraphicsCore();

	snd::FrameRate frame_rate(0);

	while (snd::System::Ref().Update())
	{
		frame_rate.Run();
		frame_rate.ShowFPS(hwnd);

		core->Reset();
		core->Barrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		
		core->SetViewport(snd::constants::kResolution);
		core->Clear(clear_color);

		core->Barrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		core->Present();
		core->WaitForCommandQueue();
	}
}