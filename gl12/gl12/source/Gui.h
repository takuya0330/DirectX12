#pragma once
#pragma comment(lib, "d3d12.lib")
#include <d3d12.h>
#include <wrl.h>
#include <functional>

namespace gl
{
	class DeviceContext;

	class Gui
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
		DeviceContext* mDeviceContext;
	public:
		Gui() = default;
		~Gui();
	public:
		void Initialize();
		void NewFrame();
		void Render();
		void WindowUpdate();
	public:

	};
}