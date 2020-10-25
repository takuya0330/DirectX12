#pragma comment(lib, "d3dcompiler.lib")

#include <d3dcompiler.h>
#include <vector>

#include "shader_compiler.hpp"
#include "../utility/utility.h"

using namespace Microsoft::WRL;

namespace re12::detail
{
	void ShaderCompiler::Initialize()
	{
		RE12_ASSERT_ERROR(SUCCEEDED(mDxcDllSupport.Initialize()), "DxcDllSupport::Initialize is failed.");
		RE12_ASSERT_ERROR(SUCCEEDED(mDxcDllSupport.CreateInstance(CLSID_DxcCompiler, mDxcCompiler3.ReleaseAndGetAddressOf())), "DxcDllSupport::CreateInstance(CLSID_DxcCompiler) is failed.");
		RE12_ASSERT_ERROR(SUCCEEDED(mDxcDllSupport.CreateInstance(CLSID_DxcUtils, mDxcUtils.ReleaseAndGetAddressOf())), "DxcDllSupport::CreateInstance(CLSID_DxcUtils) is failed.");
		RE12_ASSERT_ERROR(SUCCEEDED(mDxcUtils->CreateDefaultIncludeHandler(mDxcIncludeHandler.ReleaseAndGetAddressOf())), "IDxcUtils::CreateDefaultIncludeHandler is failed.");
	}

	ComPtr<IDxcBlob> ShaderCompiler::DXCompile(const std::wstring& _FilePath, const std::wstring& _ShaderModel)
	{
		ComPtr<IDxcBlobEncoding> lDxcBlobEncoding;
		RE12_ASSERT_ERROR(SUCCEEDED(mDxcUtils->LoadFile(_FilePath.c_str(), nullptr, lDxcBlobEncoding.GetAddressOf())), "IDxcUtils::LoadFile is failed.");

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
		RE12_ASSERT_ERROR(SUCCEEDED(mDxcCompiler3->Compile(&lDxcBuffer, lProperty.data(), static_cast<UINT32>(lProperty.size()), mDxcIncludeHandler.Get(), IID_PPV_ARGS(lDxcOperationResult.ReleaseAndGetAddressOf()))), "IDxcCompiler3::Compile is failed.");

		HRESULT hr;
		lDxcOperationResult->GetStatus(&hr);
		if (FAILED(hr))
		{
			ComPtr<IDxcBlobEncoding> lErr;
			lDxcOperationResult->GetErrorBuffer(lErr.ReleaseAndGetAddressOf());
			RE12_ASSERT_ERROR(SUCCEEDED(hr), static_cast<const char*>(lErr->GetBufferPointer()));
			return nullptr;
		}

		ComPtr<IDxcBlob> lOut;
		RE12_ASSERT_ERROR(SUCCEEDED(lDxcOperationResult->GetResult(lOut.GetAddressOf())), "IDxcOperationResult::GetResult is failed.");
		return lOut;
	}

	ComPtr<ID3DBlob> ShaderCompiler::D3DCompile(const std::wstring& _FilePath, const std::string& _ShaderModel)
	{
		UINT lCompileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		lCompileFlags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> lOut;
		ComPtr<ID3DBlob> lErr;
		HRESULT hr = D3DCompileFromFile(_FilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", _ShaderModel.c_str(), lCompileFlags, 0, lOut.GetAddressOf(), lErr.GetAddressOf());
		RE12_ASSERT_ERROR(SUCCEEDED(hr), static_cast<const char*>(lErr->GetBufferPointer()));

		return lOut;
	}
}