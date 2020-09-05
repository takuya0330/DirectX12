#pragma comment(lib, "d3dcompiler.lib")

#include <d3dcompiler.h>
#include <vector>

#include "ShaderCompiler.h"
#include "Utility.h"
#include "Mathematics.h"

using namespace Microsoft::WRL;
namespace gl
{
	void ShaderCompiler::Initialize()
	{
		_ASSERT_EXPR_A(SUCCEEDED(mDxcDllSupport.Initialize()), "DxcDllSupport::Initialize is failed.");
		_ASSERT_EXPR_A(SUCCEEDED(mDxcDllSupport.CreateInstance(CLSID_DxcCompiler, mDxcCompiler3.ReleaseAndGetAddressOf())), "DxcDllSupport::CreateInstance(CLSID_DxcCompiler) is failed.");
		_ASSERT_EXPR_A(SUCCEEDED(mDxcDllSupport.CreateInstance(CLSID_DxcUtils, mDxcUtils.ReleaseAndGetAddressOf())), "DxcDllSupport::CreateInstance(CLSID_DxcUtils) is failed.");
		_ASSERT_EXPR_A(SUCCEEDED(mDxcUtils->CreateDefaultIncludeHandler(mDxcIncludeHandler.ReleaseAndGetAddressOf())), "IDxcUtils::CreateDefaultIncludeHandler is failed.");
	}

	ComPtr<IDxcBlob> ShaderCompiler::DXCompile(const std::wstring& _Filename, const std::wstring& _ShaderModel)
	{
		ComPtr<IDxcBlobEncoding> lDxcBlobEncoding;
		_ASSERT_EXPR_A(SUCCEEDED(mDxcUtils->LoadFile(_Filename.c_str(), nullptr, lDxcBlobEncoding.GetAddressOf())), "IDxcUtils::LoadFile is failed.");
	
		DxcBuffer lDxcBuffer = {
			lDxcBlobEncoding->GetBufferPointer(),
			lDxcBlobEncoding->GetBufferSize(),
			DXC_CP_ACP,
		};

		std::vector<LPCWSTR> lProperty;
		lProperty.push_back(_Filename.c_str());
		lProperty.push_back(L"-E");
		lProperty.push_back(L"main");
		lProperty.push_back(L"-T");
		lProperty.push_back(_ShaderModel.c_str());
#ifdef _DEBUG
		lProperty.push_back(DXC_ARG_DEBUG);
		//lProperty.push_back(DXC_ARG_SKIP_OPTIMIZATIONS);
#endif
		ComPtr<IDxcOperationResult> lDxcOperationResult;
		_ASSERT_EXPR_A(SUCCEEDED(mDxcCompiler3->Compile(&lDxcBuffer, lProperty.data(), static_cast<UINT32>(lProperty.size()), mDxcIncludeHandler.Get(), IID_PPV_ARGS(lDxcOperationResult.ReleaseAndGetAddressOf()))), "IDxcCompiler3::Compile is failed.");

		HRESULT hr;
		lDxcOperationResult->GetStatus(&hr);
		if (FAILED(hr))
		{
			ComPtr<IDxcBlobEncoding> lErr;
			lDxcOperationResult->GetErrorBuffer(lErr.ReleaseAndGetAddressOf());
			_ASSERT_EXPR_A(SUCCEEDED(hr), static_cast<const char*>(lErr->GetBufferPointer()));
			return nullptr;
		}

		ComPtr<IDxcBlob> lOut;
		_ASSERT_EXPR_A(SUCCEEDED(lDxcOperationResult->GetResult(lOut.GetAddressOf())), "IDxcOperationResult::GetResult is failed.");
		return lOut;
	}

	ComPtr<ID3DBlob> ShaderCompiler::D3DCompile(const std::wstring& _Filename, const std::string& _ShaderModel)
	{
		uint lCompileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		lCompileFlags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> lOut;
		ComPtr<ID3DBlob> lErr;
		HRESULT hr = D3DCompileFromFile(_Filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", _ShaderModel.c_str(), lCompileFlags, 0, lOut.GetAddressOf(), lErr.GetAddressOf());
		_ASSERT_EXPR_A(SUCCEEDED(hr), static_cast<const char*>(lErr->GetBufferPointer()));

		return lOut;
	}
}