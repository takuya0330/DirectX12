#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>

#include "../../third_party/dxc/dxcapi.h"
#include "../../third_party/dxc/dxcapi.use.h"

namespace re12::detail
{
	class ShaderCompiler
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		dxc::DxcDllSupport mDxcDllSupport;
		ComPtr<IDxcCompiler3> mDxcCompiler3;
		ComPtr<IDxcUtils>  mDxcUtils;
		ComPtr<IDxcIncludeHandler> mDxcIncludeHandler;
	public:
		ShaderCompiler() = default;
		~ShaderCompiler() = default;
	public:
		void Initialize();
	public:
		ComPtr<IDxcBlob> DXCompile(const std::wstring& _FilePath, const std::wstring& _ShaderModel);
		ComPtr<ID3DBlob> D3DCompile(const std::wstring& _FilePath, const std::string& _ShaderModel);
	};
}