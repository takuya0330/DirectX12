#include "../third_party/imgui/imgui.h"
#include "../third_party/imgui/imgui_impl_win32.h"
#include "../third_party/imgui/imgui_impl_dx12.h"

#include "framework.h"
#include "framerate.h"
#include "administrator.h"
#include "generate_device.h"
#include "rendering_device.h"
#include "keyboard.h"
#include "mouse.h"

namespace
{
	constexpr const char* kWindowTitle = "gl";
	constexpr bool kShowCursor = true;
	constexpr float kClearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	constexpr gl::uint kMaxFPS = 60;

	HWND setup_window(WNDPROC _wndproc, HINSTANCE _hinstance, UINT _width, UINT _height)
	{
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = _wndproc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = _hinstance;
		wcex.hIcon = 0;// LoadIcon(wcex.hInstance, reinterpret_cast<LPCWSTR>(IDI_ICON1));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"gl";
		wcex.hIconSm = 0;// LoadIcon(wcex.hInstance, reinterpret_cast<LPCWSTR>(IDI_ICON1));
		RegisterClassEx(&wcex);
		RECT rect = { 0, 0, (LONG)_width, (LONG)_height };
		DWORD style = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
		AdjustWindowRect(&rect, style, false);

		return CreateWindowA("gl", kWindowTitle, style, CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(rect.right - rect.left), static_cast<int>(rect.bottom - rect.top), nullptr, nullptr, _hinstance, nullptr);
	}

	bool system_update()
	{
		MSG msg = {};
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return true;
			}
		}
		return false;
	}
}

int framework::run(WNDPROC _wndproc, HINSTANCE _hinstance, int _cmdshow)
{
	HWND hwnd = setup_window(_wndproc, _hinstance, gl::kWindowSize.x, gl::kWindowSize.y);

	ShowWindow(hwnd, _cmdshow);
	ShowCursor(kShowCursor);

	// フレームレート設定
	gl::framerate fps(kMaxFPS);

	// オブジェクト管理者生成
	gl::administrator admin(hwnd);

	gl::generate_device* generator = gl::administrator::get<gl::generate_device>();
	gl::rendering_device* renderer = gl::administrator::get<gl::rendering_device>();

	ID3D12Device* device = generator->get_device();
	ID3D12DescriptorHeap* gui_descriptor_heap = renderer->get_gui_descriptor_heap();
	ID3D12GraphicsCommandList* command_list = renderer->get_command_list();

	// imgui設定
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(device, gl::kFrameBufferNum, DXGI_FORMAT_R8G8B8A8_UNORM, gui_descriptor_heap, gui_descriptor_heap->GetCPUDescriptorHandleForHeapStart(), gui_descriptor_heap->GetGPUDescriptorHandleForHeapStart());

	gl::keyboard* key = gl::administrator::get<gl::keyboard>();
	gl::mouse* mouse = gl::administrator::get<gl::mouse>();

	// ゲームループ
	while (system_update())
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		fps.run();
		fps.show(hwnd);

		mouse->update();

		if (ImGui::Begin("test"))
		{
			ImGui::Text("aaaaa"); 
			ImGui::Text("x : %d, y : %d", mouse->get_pos().x, mouse->get_pos().y);
			ImGui::Text("wheel : %d", mouse->get_wheel());
		}
		ImGui::End();

		renderer->reset();
		renderer->barrier_transition(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		
		renderer->set_viewport(gl::kWindowSize);
		renderer->screen_clear(kClearColor);
		renderer->set_gui_descriptor_heap();

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
		
		renderer->barrier_transition(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);	
		renderer->execute();

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault(NULL, (void*)command_list);
		}

		renderer->present();
		renderer->wait_previous_frame();
	}

	// imgui解放
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
