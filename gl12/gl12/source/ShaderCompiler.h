#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl.h>
#include <string>

#include "../external/dxc/dxcapi.h"
#include "../external/dxc/dxcapi.use.h"

namespace gl
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
		ComPtr<IDxcBlob> DXCompile(const std::wstring& _Filename, const std::wstring& _ShaderModel);
		ComPtr<ID3DBlob> D3DCompile(const std::wstring& _Filename, const std::string& _ShaderModel);
	};
}