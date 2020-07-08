//#include "system.h"
//
//void System::Initialize(HWND hwnd)
//{	
//	d3d12core_.Initialize(hwnd);
//}
//
//void System::UnInitialize()
//{
//	d3d12core_.UnInitialize();
//}
//
//void System::Update()
//{
//
//}
//
//void System::Render()
//{
//	d3d12core_.Reset();
//	d3d12core_.Barrier(
//		D3D12_RESOURCE_STATE_PRESENT,
//		D3D12_RESOURCE_STATE_RENDER_TARGET);
//
//	d3d12core_.ClearScreen();
//
//	d3d12core_.Barrier(
//		D3D12_RESOURCE_STATE_RENDER_TARGET,
//		D3D12_RESOURCE_STATE_PRESENT);
//
//	d3d12core_.Present();
//	d3d12core_.WaitGPU();
//}
