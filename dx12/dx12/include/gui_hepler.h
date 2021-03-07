#pragma once
#pragma comment(lib, "d3d12.lib")

#include <d3d12.h>
#include <wrl.h>

class GuiHelper
{
public:
	GuiHelper() = default;
	~GuiHelper() = default;

	bool Initialize(HWND _hwnd, ID3D12Device* _device, UINT _buffer_count);
	void Terminate();
	void Update();
	void Render(ID3D12GraphicsCommandList* _command_list);
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
};