#pragma once

namespace gl
{
	class Window;
	class Keyboard;
	class Mouse;
	class XInput;
	class Device;
	class DeviceContext;
	//class ShaderCompiler;
}

template<class T>
class Component
{
private:
	T* mPtr = nullptr;
public:
	Component() { mPtr = new T(); }
public:
	void Release() { delete mPtr; mPtr = nullptr; }
public:
	T* Get() { return mPtr; }
	T* operator->() { return mPtr; }
};

class Administrator
{
	inline static Administrator* mAdmin = nullptr;
private:
	Component<gl::Window> mWindow;
	Component<gl::Keyboard> mKeyboard;
	Component<gl::Mouse> mMouse;
	Component<gl::XInput> mXInput;
	Component<gl::Device> mDevice;
	Component<gl::DeviceContext> mDeviceContext;
	//Component<gl::ShaderCompiler> mShaderCompiler;
public:
	Administrator();
	~Administrator();
public:
	inline static bool IsActive()noexcept { return mAdmin; }
public:
	template<class T> [[nodiscard]] static T* Get() { return nullptr; }
	template<> [[nodiscard]] static gl::Window* Get() { return mAdmin->mWindow.Get(); }
	template<> [[nodiscard]] static gl::Keyboard* Get() { return mAdmin->mKeyboard.Get(); }
	template<> [[nodiscard]] static gl::Mouse* Get() { return mAdmin->mMouse.Get(); }
	template<> [[nodiscard]] static gl::XInput* Get() { return mAdmin->mXInput.Get(); }
	template<> [[nodiscard]] static gl::Device* Get() { return mAdmin->mDevice.Get(); }
	template<> [[nodiscard]] static gl::DeviceContext* Get() { return mAdmin->mDeviceContext.Get(); }
	//template<> [[nodiscard]] static gl::ShaderCompiler* Get() { return mAdmin->mShaderCompiler.Get(); }
};
