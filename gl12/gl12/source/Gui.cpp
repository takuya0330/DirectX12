#include "Gui.h"
#include "Administrator.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "Utility.h"

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx12.h"

namespace gl
{
	Gui::~Gui()
	{
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void Gui::Initialize()
	{
		HWND lHwnd = Administrator::Get<Window>()->GetHwnd();
		ID3D12Device* lDevice = Administrator::Get<Device>()->GetDevice5();
		mDeviceContext = Administrator::Get<DeviceContext>();

		constexpr D3D12_DESCRIPTOR_HEAP_DESC lGuiHeapDesc{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			.NumDescriptors = 1,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
			.NodeMask = 0
		};
		_ASSERT_EXPR_A(SUCCEEDED(lDevice->CreateDescriptorHeap(&lGuiHeapDesc, IID_PPV_ARGS(mDescriptorHeap.GetAddressOf()))), "CreateDescriptorHeap is failed.");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& lInfo = ImGui::GetIO(); (void)lInfo;
		lInfo.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		lInfo.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 18.0f, NULL, lInfo.Fonts->GetGlyphRangesJapanese());

		ImGui::StyleColorsDark();

		ImGuiStyle& lStyle = ImGui::GetStyle();
		if (lInfo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			lStyle.WindowRounding = 0.0f;
			lStyle.FrameRounding = 1;
			lStyle.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplWin32_Init(lHwnd);
		ImGui_ImplDX12_Init(lDevice, DeviceContext::kBackBufferNum, DXGI_FORMAT_R8G8B8A8_UNORM, mDescriptorHeap.Get(), mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), mDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	}

	void Gui::NewFrame()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void Gui::Render()
	{
		ID3D12GraphicsCommandList* lCmdList = mDeviceContext->GetCommandList();
		lCmdList->SetDescriptorHeaps(1, mDescriptorHeap.GetAddressOf());
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), lCmdList);
	}

	void Gui::WindowUpdate()
	{
		// Update and Render additional Platform Windows
		if (ImGuiIO& lInfo = ImGui::GetIO(); lInfo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(NULL, (void*)mDeviceContext->GetCommandList());
		}
	}
}