#include <Windows.h>
#include <assert.h>

#include "Administrator.h"
#include "Windows/Window.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/XInput.h"
#include "Core/Device.h"
#include "Core/DeviceContext.h"
//#include "ShaderCompiler.h"

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
	//mShaderCompiler->Initialize();
}

Administrator::~Administrator()
{
	mWindow.Release();
	mKeyboard.Release();
	mMouse.Release();
	mXInput.Release();
	mDevice.Release();
	mDeviceContext.Release();
	//mShaderCompiler.Release();

	mAdmin = nullptr;
}
