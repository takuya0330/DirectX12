//#pragma once
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")
//
//#include <d3d12.h>
//#include <dxgi1_6.h>
//#include <wrl.h>
//
//#include <vector>
//
//#include "../library/d3dx12/d3dx12.h"
//
//class D3D12Core
//{
//public:
//	D3D12Core() {}
//	~D3D12Core() { UnInitialize(); }
//
//	void Initialize(HWND hwnd);
//	void UnInitialize();
//
//	void Reset();
//	void Barrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
//	void ClearScreen();
//	void Present();
//	void WaitGPU();
//
//public:
//	ID3D12Device* GetDevice()const { return device_.Get(); }
//	ID3D12GraphicsCommandList* GetGraphicsCommandList()const { return graphics_command_list_.Get(); }
//	IDXGISwapChain4* GetSwapChain()const { return swap_chain_.Get(); }
//
//private:
//	Microsoft::WRL::ComPtr<ID3D12Device> device_;
//	Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue_;
//	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator_;
//	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> graphics_command_list_;
//
//	struct RenderTarget
//	{
//		static constexpr UINT kFrameCount = 3;
//		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	descriptor_heap_;
//		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resource_;
//		UINT descriptor_size_;
//	} render_target_;
//
//	struct DepthStencil
//	{
//		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
//		Microsoft::WRL::ComPtr<ID3D12Resource>	resource_;
//	} depth_stencil_;
//
//	UINT frame_index_;
//	Microsoft::WRL::ComPtr<IDXGISwapChain4> swap_chain_;
//
//	HANDLE hfence_;
//	UINT64 fence_value_;
//	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
//
//	CD3DX12_VIEWPORT viewport_;
//	CD3DX12_RECT scissor_;
//};
