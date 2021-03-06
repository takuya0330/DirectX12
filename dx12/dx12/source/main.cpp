#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_dx12.h"
#include "../thirdparty/imgui/imgui_impl_win32.h"

#include "../include/utility.h"
#include "../include/window.h"
#include "../include/timer.h"
#include "../include/d3d12_manager.h"
#include "../include/gui_hepler.h"
#include "../include/graphics_state.h"
#include "../include/pipeline_manager.h"
#include "../include/triangle.h"

class Engine
{
public:
	Engine()
	{

	}
	~Engine()
	{

	}
private:
	inline static Engine* ptr_ = nullptr;
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case VK_F11:
			break;
		}
	}
	break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int main()
{
	constexpr const float kClearColor[] = { 0,0,0,1 };

	WNDCLASSEX wcex = {
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = WndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = GetModuleHandle(NULL),
		.hIcon = 0,
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		.lpszMenuName = NULL,
		.lpszClassName = L"Title",
		.hIconSm = 0
	};
	RegisterClassEx(&wcex);

	Window window;
	window.Initialize("DirectX12 Test", 1280, 720);
	Debug::Console("Successed : Window::Initialize()\n");

	Timer timer;
	timer.Initialize(0);
	Debug::Console("Successed : Timer::Initialize()\n");

	D3D12Manager d3d12;
	if (!d3d12.Initialize(window.Hwnd, window.Width, window.Height)) return 0;
	Debug::Console("Successed : D3D12Manager::Initialize()\n");

	GuiHelper gui;
	if (!gui.Initialize(window.Hwnd, d3d12.Device, kFrameBufferCount, DXGI_FORMAT_R8G8B8A8_UNORM)) return 0;
	Debug::Console("Successed : GuiHelper::Initialize()\n");

	PipelineManager pso;
	ComPtr<ID3DBlob> vertex_shader = d3d12.D3DCompile(L"shader/triangle_vs.hlsl", "vs_5_0");
	ComPtr<ID3DBlob> pixel_shader = d3d12.D3DCompile(L"shader/triangle_ps.hlsl", "ps_5_0");
	if (!pso.Initialize(
		"Triangle",
		d3d12.Device,
		nullptr, 0,
		nullptr, 0,
		{ vertex_shader->GetBufferPointer(),vertex_shader->GetBufferSize() },
		{}, {}, {},
		{ pixel_shader->GetBufferPointer(),pixel_shader->GetBufferSize() },
		BlendState(BlendMode::eAlpha),
		RasterizerState(RasterizerMode::e2D),
		DepthStencilState(DepthStencilMode::e2D),
		{ Triangle::kInputElement,Triangle::kInputElementNum })) return 0;
	Debug::Console("Successed : PipelineManager::Initialize()\n");

	Triangle triangle;
	if (!triangle.Initialize(d3d12.Device)) return 0;
	Debug::Console("Successed : Triangle::Initialize()\n");

	while (window.Update())
	{
		timer.Update();
		gui.Update();

		if (ImGui::Begin("Info"))
		{
			UINT fps = timer.FPS;
			float delta_time = timer.DeltaTime;
			ImGui::Text("FPS : %d / DeltaTime : %f", fps, delta_time);
		}
		ImGui::End();

		d3d12.SetFrameIndex();
		d3d12.Reset();
		d3d12.BarrierRenderTarget();
		d3d12.ClearRenderTargetView(kClearColor);
		d3d12.ClearDepthStencilView();
		d3d12.SetRenderTargets();
		d3d12.SetViewport(window.Width, window.Height);

		pso.Bind(d3d12.GraphicsCommandList, "Triangle");

		triangle.Draw(d3d12.GraphicsCommandList);

		gui.Render(d3d12.GraphicsCommandList);
		
		d3d12.BarrierPresent();
		d3d12.ExecuteCommandQueue();
		d3d12.Present(0);
		d3d12.WaitPreviousFrame();
	}

	gui.Terminate();

	return 0;
}