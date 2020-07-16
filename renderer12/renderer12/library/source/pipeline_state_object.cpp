#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

#include "pipeline_state_object.h"
#include "utility.h"

using namespace Microsoft::WRL;
namespace snd
{
	bool PipelineStateObject::CompileShader(const wchar_t* _filename, const char* _shadermodel, ID3DBlob** _blob)
	{
		u_int compile_flag = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined _DEBUG | DEBUG
		compile_flag |= D3DCOMPILE_DEBUG;
#endif

		ComPtr<ID3DBlob> msg;
		HRESULT hr = D3DCompileFromFile(_filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", _shadermodel, compile_flag, 0, _blob, msg.GetAddressOf());
		if (FAILED(hr))
		{
			OutputDebugStringA((char*)msg->GetBufferPointer());
			ASSERT_MESSAGE(SUCCEEDED(hr), "file not found.");
			return false;
		}
		return true;
	}

	bool PipelineStateObject::CompileShader(const wchar_t* _filename, const char* _entrypoint, const char* _shadermodel, ID3DBlob** _blob)
	{
		u_int compile_flag = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined _DEBUG | DEBUG
		compile_flag |= D3DCOMPILE_DEBUG;
#endif

		ComPtr<ID3DBlob> msg;
		HRESULT hr = D3DCompileFromFile(_filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _entrypoint, _shadermodel, compile_flag, 0, _blob, msg.GetAddressOf());
		if (FAILED(hr))
		{
			OutputDebugStringA((char*)msg->GetBufferPointer());
			ASSERT_MESSAGE(SUCCEEDED(hr), "file not found.");
			return false;
		}
		return true;
	}
}