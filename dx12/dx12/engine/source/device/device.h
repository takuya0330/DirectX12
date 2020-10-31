#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <string>

#include "../../third_party/dxc/dxcapi.h"
#include "../../third_party/dxc/dxcapi.use.h"

namespace detail
{
	class Device
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		bool mUseWrapAdapter = false;
		ComPtr<ID3D12Device5> mDevice5;
		ComPtr<IDXGIFactory6> mFactory6;
	private:
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
	private:
		ComPtr<ID3D12Fence> mFence;
		UINT64 mFenceValue = 0;
		HANDLE mFenceEvent = 0;
	private:
		dxc::DxcDllSupport mDxcDllSupport;
		ComPtr<IDxcCompiler3> mDxcCompiler3;
		ComPtr<IDxcUtils>  mDxcUtils;
		ComPtr<IDxcIncludeHandler> mDxcIncludeHandler;
	public:
		Device() = default;
		~Device() = default;
	public:
		void Initialize();
		void ResetCommand();
		void BarrierTransition(ID3D12Resource* _Resource, D3D12_RESOURCE_STATES _Before, D3D12_RESOURCE_STATES _Affter);
		void ExecuteCommandList();
		void WaitFence();
		IDxcBlob* DXCompile(const std::wstring& _FilePath, const std::wstring& _ShaderModel);
		ID3DBlob* D3DCompile(const std::wstring& _FilePath, const std::string& _ShaderModel);
	public:
		ID3D12Device5* GetDevice5()const { return mDevice5.Get(); }
		IDXGIFactory6* GetFactory6()const { return mFactory6.Get(); }
		ID3D12CommandQueue* GetCommandQueue()const { return mCommandQueue.Get(); }
		ID3D12CommandAllocator* GetCommandAllocator()const { return mCommandAllocator.Get(); }
		ID3D12GraphicsCommandList* GetCommandList()const { return mCommandList.Get(); }
	};
}