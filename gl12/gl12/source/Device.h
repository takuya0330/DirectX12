#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "Mathematics.h"

namespace gl
{
	class Device
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		bool mUseWrapAdapter = false;
		ComPtr<ID3D12Device5> mDevice5;
		ComPtr<IDXGIFactory6> mFactory6;
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
	private:
		void CreateDebugLayer();
		void CreateInfoQueue();
		void CreateFactory(uint _DxgiFlags);
		void CreateDevice(IDXGIAdapter4* _Adapter);
		void CreateCommandQueue();
		void CreateCommandAllocator();
		void CreateCommandList();
		ComPtr<IDXGIAdapter4> SearchAdapter(IDXGIFactory6* _factory);
	public:
		Device() = default;
		~Device() = default;
	public:
		void Initialize();
		void ReportLiveObjects();
	public:
		ID3D12Device5* GetDevice5()const { return mDevice5.Get(); }
		IDXGIFactory6* GetFactory6()const { return mFactory6.Get(); }
		ID3D12CommandQueue* GetCommandQueue()const { return mCommandQueue.Get(); }
		ID3D12CommandAllocator* GetCommandAllocator()const { return mCommandAllocator.Get(); }
		ID3D12GraphicsCommandList* GetCommandList()const { return mCommandList.Get(); }
	};
}