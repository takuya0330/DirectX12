//#pragma once
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")
//
//#include <d3d12.h>
//#include <wrl.h>
//
//#include "../library/d3dx12/d3dx12.h"
//
//#include "math.h"
//
//class Texture
//{
//public:
//	Texture() {}
//	Texture(ID3D12Device* device);
//	Texture(ID3D12Device* device, const char* filename);
//	~Texture() {}
//
//	struct Vertex
//	{
//		float3 position_;
//		float2 uv_;
//	};
//
//private:
//	Microsoft::WRL::ComPtr<ID3D12Resource>  vertex_buffer_;
//	D3D12_VERTEX_BUFFER_VIEW					 vertex_buffer_view_;
//};