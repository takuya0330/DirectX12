#include "core.h"
#include "device.h"
#include "renderer.h"
#include "scene.h"

#include <memory>

void Main()
{
	constexpr float clear_color[] = { 1,1,0,1 };
	snd::detail::Renderer* renderer = snd::Core::Ref().GetRenderer();

	//std::unique_ptr<snd::detail::Scene> current_scene;
	//current_scene->Initialize();

	while (snd::Core::Ref().Update())
	{
		renderer->Reset();
		renderer->Barrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		renderer->Clear(clear_color);
		renderer->SetViewport(snd::constants::kResolution);
		renderer->Barrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		renderer->Present();
		renderer->WaitGPU();
	}
}