#pragma once
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

namespace snd::detail
{
	class Device
	{
	public:
		Device() {}
		~Device() {}

		void Create();

		ID3D12Device* GetD3D12Device()const { return device_.Get(); }
		IDXGIFactory6* GetDXGIFactory()const { return factory_.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D12Device> device_;
		Microsoft::WRL::ComPtr<IDXGIFactory6> factory_;
	};
}