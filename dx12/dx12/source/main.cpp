#include "../header/engine.h"
#include "../header/window.h"
#include "../header/Dx12Test.h"
//#include "../header/d3d12_manager.h"
#include "../header/timer.h"

void Main()
{
	Window* window = Engine::Get<Window>();
	HWND hwnd = window->GetHwnd();
	Dx12Test* dx12 = Engine::Get<Dx12Test>();
	//D3D12Manager* d3d12 = Engine::Get<D3D12Manager>();

	Timer timer;
	timer.Initialize(0);

	while (window->Update())
	{
		timer.Update();
		timer.Show(hwnd);

		dx12->RenderBegin();
		//d3d12->RenderBegin();

		dx12->RenderEnd();
		//d3d12->RenderEnd();
	}
	dx12->WaitFence();
	//d3d12->WaitIdle();
}