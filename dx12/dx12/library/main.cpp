#include "core.h"
#include "device.h"
#include "renderer.h"
#include "scene.h"

#include <memory>

#include "texture.h"
#include "pipeline_state.h"
#include "root_signature.h"
#include "frame_rate.h"

void Main()
{
	constexpr float clear_color[] = { 0,0,0,1 };

	const HWND hwnd = snd::Core::Ref().GetHWND();
	snd::detail::Device* device = snd::Core::Ref().GetDevice();
	snd::detail::Renderer* renderer = snd::Core::Ref().GetRenderer();

	snd::detail::FrameRate frame_rate(0);

	Microsoft::WRL::ComPtr<ID3DBlob> vs, ps;
	device->CompileShader(L"hlsl/texture_vs.hlsl", "main", "vs_5_0", vs.GetAddressOf());
	device->CompileShader(L"hlsl/texture_ps.hlsl", "main", "ps_5_0", ps.GetAddressOf());

	//snd::Texture texture1(device->GetD3D12Device());
	//snd::Texture texture2(device->GetD3D12Device());
	//
	//snd::RootSignature root_signature;
	//root_signature.Create(device->GetD3D12Device(), 0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//
	//snd::PipelineState pipeline_state;
	//pipeline_state.SetVertexShader(vs.Get());
	//pipeline_state.SetPixelShader(ps.Get());
	//pipeline_state.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
	//pipeline_state.SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT));
	//pipeline_state.SetDepthStencil(DXGI_FORMAT_D32_FLOAT, CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
	//pipeline_state.SetRenderTargets(1, DXGI_FORMAT_R8G8B8A8_UNORM);
	//pipeline_state.SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	//pipeline_state.SetInputLayout(snd::Texture::kInputElement, snd::Texture::kInputElementNum);
	//pipeline_state.Setup(device->GetD3D12Device(), root_signature);

	float angle = 0;
	while (snd::Core::Ref().Update())
	{
		frame_rate.Run();
		frame_rate.ShowFPS(hwnd);

		renderer->Reset();
		renderer->Barrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		renderer->Clear(clear_color);
		renderer->SetViewport(snd::constants::kResolution);

		//angle += 0.1f;
		//texture1.Update(10, 100, 0, 100, angle);
		//renderer->Draw(texture1, pipeline_state, root_signature);
		//
		//texture2.Update(200, 100, 0, 100, angle * -1);
		//renderer->Draw(texture2, pipeline_state, root_signature);

		renderer->Barrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		renderer->Present(0);
		renderer->WaitGPU();
	}
}