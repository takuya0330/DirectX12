#pragma once

namespace gl::detail
{
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
}

namespace gl
{
	class Window;
	class Keyboard;
	class Mouse;
	class XInput;
	class Device;
	class DeviceContext;
	class Gui;
	class ShaderCompiler;

	class Administrator
	{
		inline static Administrator* mAdmin = nullptr;
	private:
		detail::Component<Window> mWindow;
		detail::Component<Keyboard> mKeyboard;
		detail::Component<Mouse> mMouse;
		detail::Component<XInput> mXInput;
		detail::Component<Device> mDevice;
		detail::Component<DeviceContext> mDeviceContext;
		detail::Component<Gui> mGui;
		detail::Component<ShaderCompiler> mShaderCompiler;
	public:
		Administrator();
		~Administrator();
	public:
		inline static bool IsActive()noexcept { return mAdmin; }
	public:
		template<class T> [[nodiscard]] static T* Get() { return nullptr; }
		template<> [[nodiscard]] static Window* Get() { return mAdmin->mWindow.Get(); }
		template<> [[nodiscard]] static Keyboard* Get() { return mAdmin->mKeyboard.Get(); }
		template<> [[nodiscard]] static Mouse* Get() { return mAdmin->mMouse.Get(); }
		template<> [[nodiscard]] static XInput* Get() { return mAdmin->mXInput.Get(); }
		template<> [[nodiscard]] static Device* Get() { return mAdmin->mDevice.Get(); }
		template<> [[nodiscard]] static DeviceContext* Get() { return mAdmin->mDeviceContext.Get(); }
		template<> [[nodiscard]] static Gui* Get() { return mAdmin->mGui.Get(); }
		template<> [[nodiscard]] static ShaderCompiler* Get() { return mAdmin->mShaderCompiler.Get(); }
	};
}
