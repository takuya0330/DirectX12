#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <iostream>

namespace snd
{
	class Device
	{
	public:
		Device() {}
		~Device() {}

		void Create();
		bool CompileShader(const std::wstring& _name, const std::string& _entry_point, const std::string& _shader_model, ID3DBlob** _blob);

		ID3D12Device* Get()const { return device_.Get(); }
		IDXGIFactory6* GetFactory()const { return factory_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12Device> device_;
		Microsoft::WRL::ComPtr<IDXGIFactory6> factory_;
	};
}