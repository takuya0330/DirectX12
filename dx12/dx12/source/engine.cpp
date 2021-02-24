#include <assert.h>

#include "../header/window.h"
#include "../header/Dx12Test.h"
//#include "../header/d3d12_manager.h"

#include "../header/engine.h"

Engine::Engine()
{
	assert(!engine);
	engine = this;

	window->Initialize();
	dx12->Initialize(window.Get()->GetHwnd());
	//d3d12->Initialize(window.Get()->GetHwnd());
}

Engine::~Engine()
{
	window.Release();
	dx12.Release();
	//d3d12.Release();

	engine = nullptr;
}