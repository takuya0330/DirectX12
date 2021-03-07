#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_dx12.h"
#include "../thirdparty/imgui/imgui_impl_win32.h"

#include "../include/gui_hepler.h"
#include "../include/d3d12_helper.h"
#include "../include/utility.h"

bool GuiHelper::Initialize(HWND _hwnd, ID3D12Device* _device, UINT _buffer_count)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	auto hr = cd3d12::CreateDescriptorHeap(
		_device, 
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 
		1, 
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 
		&descriptor_heap_);
	if (FAILED(hr))
	{
		debug::Console("Failed : CreateDescriptorHeap\n");
		debug::Console(OUTPUT_FILE " : " OUTPUT_LINE "\n");
		return false;
	}

	ImGui_ImplWin32_Init(_hwnd);
	ImGui_ImplDX12_Init(
		_device, 
		_buffer_count, 
		DXGI_FORMAT_R8G8B8A8_UNORM,
		descriptor_heap_.Get(), 
		descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), 
		descriptor_heap_->GetGPUDescriptorHandleForHeapStart());

	return true;
}

void GuiHelper::Terminate()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GuiHelper::Update()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GuiHelper::Render(ID3D12GraphicsCommandList* _command_list)
{
	_command_list->SetDescriptorHeaps(1, descriptor_heap_.GetAddressOf());

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), _command_list);

	if (ImGuiIO& io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault(NULL, (void*)_command_list);
	}
}