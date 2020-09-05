#include <Windows.h>
#include <assert.h>

#include "Administrator.h"
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "XInput.h"
#include "Device.h"
#include "DeviceContext.h"
#include "Gui.h"
#include "ShaderCompiler.h"

namespace gl
{
	Administrator::Administrator()
	{
		assert(!mAdmin);
		mAdmin = this;

		mWindow->Initialize();
		mKeyboard->Initialize();
		mMouse->Initialize();
		mXInput->Initialize();
		mDevice->Initialize();
		mDeviceContext->Initialize();
		mGui->Initialize();
		mShaderCompiler->Initialize();
	}

	Administrator::~Administrator()
	{
		mWindow.Release();
		mKeyboard.Release();
		mMouse.Release();
		mXInput.Release();
		mDevice.Release();
		mDeviceContext.Release();
		mGui.Release();
		mShaderCompiler.Release();

		mAdmin = nullptr;
	}
}