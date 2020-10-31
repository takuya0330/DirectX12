#pragma comment(lib, "d3dcompiler.lib")

#include <d3dcompiler.h>
#include <vector>

#include "device.h"
#include "../utility/utility.h"

namespace detail
{
	void Device::Initialize()
	{
		HRESULT hr = S_OK;
#if _DEBUG
		constexpr UINT lDXGIFlags = DXGI_CREATE_FACTORY_DEBUG;
		ComPtr<ID3D12Debug> lDebug;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(lDebug.GetAddressOf()))))
			lDebug->EnableDebugLayer();
#else
		constexpr uint lDXGIFlags = 0;
#endif
		ComPtr<IDXGIFactory2> lFactory2;
		hr = CreateDXGIFactory2(lDXGIFlags, IID_PPV_ARGS(lFactory2.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "CreateDXGIFactory2 is failed.");
		lFactory2.As(&mFactory6);

		ComPtr<IDXGIAdapter4> lAdapter;
		DXGI_ADAPTER_DESC3 lAdapterDesc{};
		for (UINT i = 0;; ++i)
		{
			if (mFactory6->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(lAdapter.GetAddressOf())) == DXGI_ERROR_NOT_FOUND)
			{
				mUseWrapAdapter = true;
				break;
			}

			lAdapter->GetDesc3(&lAdapterDesc);

			if (!(lAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE))
				break;
		}
		if (mUseWrapAdapter)
		{
			hr = mFactory6->EnumWarpAdapter(IID_PPV_ARGS(lAdapter.GetAddressOf()));
			_ERROR_TRACE(SUCCEEDED(hr), "IDXGIFactory6::EnumWarpAdapter is failed.");
		}

		ComPtr<ID3D12Device> lDevice;
		hr = D3D12CreateDevice(lAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(lDevice.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "D3D12CreateDevice is failed.");
		lDevice.As(&mDevice5);
#if _DEBUG
		ComPtr<ID3D12InfoQueue> lInfoQueue;
		if (SUCCEEDED(mDevice5->QueryInterface(IID_PPV_ARGS(lInfoQueue.GetAddressOf()))))
		{
			D3D12_MESSAGE_SEVERITY lSevirity[] = {
				D3D12_MESSAGE_SEVERITY_INFO
			};
			D3D12_MESSAGE_ID lDenyID[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
				D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
			};
			D3D12_INFO_QUEUE_FILTER lFilter{
				.DenyList = {
					.NumSeverities = _countof(lSevirity),
					.pSeverityList = lSevirity,
					.NumIDs = _countof(lDenyID),
					.pIDList = lDenyID
				}
			};
			lInfoQueue->PushStorageFilter(&lFilter);
		}
#endif
		constexpr D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{
			.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = 0
		};
		hr = mDevice5->CreateCommandQueue(&lCommandQueueDesc, IID_PPV_ARGS(mCommandQueue.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateCommandQueue is failed.");

		hr = mDevice5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateCommandAllocator is failed.");

		hr = mDevice5->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(mCommandList.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateCommandList is failed.");
		mCommandList->Close();

		hr = mDevice5->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
		_ERROR_TRACE(SUCCEEDED(hr), "ID3D12Device5::CreateFence is failed.");
		mFenceEvent = CreateEvent(nullptr, false, false, nullptr);
		mFenceValue = 0;

		hr = mDxcDllSupport.Initialize();
		_ERROR_TRACE(SUCCEEDED(hr), "DxcDllSupport::Initialize is failed.");

		hr = mDxcDllSupport.CreateInstance(CLSID_DxcCompiler, mDxcCompiler3.ReleaseAndGetAddressOf());
		_ERROR_TRACE(SUCCEEDED(hr), "DxcDllSupport::CreateInstance(CLSID_DxcCompiler) is failed.");

		hr = mDxcDllSupport.CreateInstance(CLSID_DxcUtils, mDxcUtils.ReleaseAndGetAddressOf());
		_ERROR_TRACE(SUCCEEDED(hr), "DxcDllSupport::CreateInstance(CLSID_DxcUtils) is failed.");

		hr = mDxcUtils->CreateDefaultIncludeHandler(mDxcIncludeHandler.ReleaseAndGetAddressOf());
		_ERROR_TRACE(SUCCEEDED(hr), "IDxcUtils::CreateDefaultIncludeHandler is failed.");
	}

	void Device::ResetCommand()
	{
		mCommandAllocator->Reset();
		mCommandList->Reset(mCommandAllocator.Get(), nullptr);
	}

	void Device::BarrierTransition(ID3D12Resource* _Resource, D3D12_RESOURCE_STATES _Before, D3D12_RESOURCE_STATES _Affter)
	{
		D3D12_RESOURCE_BARRIER lBarrier{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
			.Transition = {
				.pResource = _Resource,
				.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
				.StateBefore = _Before,
				.StateAfter = _Affter
			}
		};
		mCommandList->ResourceBarrier(1, &lBarrier);
	}

	void Device::ExecuteCommandList()
	{
		mCommandList->Close();
		mCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)mCommandList.GetAddressOf());
	}

	void Device::WaitFence()
	{
		mCommandQueue->Signal(mFence.Get(), ++mFenceValue);
		if (mFence->GetCompletedValue() != mFenceValue)
		{
			mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
			WaitForSingleObject(mFenceEvent, INFINITE);
		}
	}

	IDxcBlob* Device::DXCompile(const std::wstring& _FilePath, const std::wstring& _ShaderModel)
	{
		ComPtr<IDxcBlobEncoding> lDxcBlobEncoding;
		_ERROR_TRACE(SUCCEEDED(mDxcUtils->LoadFile(_FilePath.c_str(), nullptr, lDxcBlobEncoding.GetAddressOf())), "IDxcUtils::LoadFile is failed.");

		DxcBuffer lDxcBuffer = {
			lDxcBlobEncoding->GetBufferPointer(),
			lDxcBlobEncoding->GetBufferSize(),
			DXC_CP_ACP,
		};

		std::vector<LPCWSTR> lProperty;
		lProperty.push_back(_FilePath.c_str());
		lProperty.push_back(L"-E");
		lProperty.push_back(L"main");
		lProperty.push_back(L"-T");
		lProperty.push_back(_ShaderModel.c_str());
#ifdef _DEBUG
		lProperty.push_back(DXC_ARG_DEBUG);
		//lProperty.push_back(DXC_ARG_SKIP_OPTIMIZATIONS);
#endif
		ComPtr<IDxcOperationResult> lDxcOperationResult;
		_ERROR_TRACE(SUCCEEDED(mDxcCompiler3->Compile(&lDxcBuffer, lProperty.data(), static_cast<UINT32>(lProperty.size()), mDxcIncludeHandler.Get(), IID_PPV_ARGS(lDxcOperationResult.ReleaseAndGetAddressOf()))), "IDxcCompiler3::Compile is failed.");

		HRESULT hr;
		lDxcOperationResult->GetStatus(&hr);
		if (FAILED(hr))
		{
			ComPtr<IDxcBlobEncoding> lErr;
			lDxcOperationResult->GetErrorBuffer(lErr.ReleaseAndGetAddressOf());
			_ERROR_TRACE(SUCCEEDED(hr), static_cast<const char*>(lErr->GetBufferPointer()));
			return nullptr;
		}

		ComPtr<IDxcBlob> lOut;
		_ERROR_TRACE(SUCCEEDED(lDxcOperationResult->GetResult(lOut.GetAddressOf())), "IDxcOperationResult::GetResult is failed.");
		return lOut.Get();
	}

	ID3DBlob* Device::D3DCompile(const std::wstring& _FilePath, const std::string& _ShaderModel)
	{
		UINT lCompileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		lCompileFlags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> lOut;
		ComPtr<ID3DBlob> lErr;
		HRESULT hr = D3DCompileFromFile(_FilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", _ShaderModel.c_str(), lCompileFlags, 0, lOut.GetAddressOf(), lErr.GetAddressOf());
		_ERROR_TRACE(SUCCEEDED(hr), static_cast<const char*>(lErr->GetBufferPointer()));

		return lOut.Get();
	}
}