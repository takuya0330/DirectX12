#include <assert.h>

#include "administrator.hpp"
#include "../utility/utility.h"
#include "../window/window.hpp"
#include "../device/device.hpp"
#include "../device_context/device_context.hpp"
#include "../shader_compiler/shader_compiler.hpp"
#include "../keyboard/keyboard.hpp"
#include "../mouse/mouse.hpp"

namespace re12
{
	Administrator::Administrator()
	{
		// ˆê‚Â‚µ‚©¶¬‚Å‚«‚È‚¢
		assert(!mPtr);
		mPtr = this;

		mWindow->Initialize(1280, 720);
		RE12_OUTPUT_CONSOLE("re12::Window Initialized.");

		mDevice->Initialize();
		RE12_OUTPUT_CONSOLE("re12::Device Initialized.");

		mDeviceContext->Initialize();
		RE12_OUTPUT_CONSOLE("re12::DeviceContext Initialized.");

		mShaderCompiler->Initialize();
		RE12_OUTPUT_CONSOLE("re12::ShaderCompiler Initialized.");

		mKeyboard->Initialize();
		RE12_OUTPUT_CONSOLE("re12::Keyboard Initialized.");

		mMouse->Initialize();
		RE12_OUTPUT_CONSOLE("re12::Mouse Initialized.");
	}

	Administrator::~Administrator()
	{
		mWindow.Release();
		mDevice.Release();
		mDeviceContext.Release();
		mShaderCompiler.Release();
		mKeyboard.Release();
		mMouse.Release();

		mPtr = nullptr;

		RE12_OUTPUT_CONSOLE("Administrator::~Administrator called.");
	}
}