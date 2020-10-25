#pragma once

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

namespace re12::detail
{
	class Window;
	class Device;
	class DeviceContext;
	class ShaderCompiler;
	class Keyboard;
	class Mouse;
}

namespace re12
{
	class Administrator
	{
		inline static Administrator* mPtr = nullptr;
	private:
		Component<re12::detail::Window> mWindow;
		Component<re12::detail::Device> mDevice;
		Component<re12::detail::DeviceContext> mDeviceContext;
		Component<re12::detail::ShaderCompiler> mShaderCompiler;
		Component<re12::detail::Keyboard> mKeyboard;
		Component<re12::detail::Mouse> mMouse;
	public:
		Administrator();
		~Administrator();
	public:
		inline static bool IsActive()noexcept { return mPtr; }
	public:
		template<class T> [[nodiscard]] static T* Get() { return nullptr; }
		template<> [[nodiscard]] static re12::detail::Window* Get() { return mPtr->mWindow.Get(); }
		template<> [[nodiscard]] static re12::detail::Device* Get() { return mPtr->mDevice.Get(); }
		template<> [[nodiscard]] static re12::detail::DeviceContext* Get() { return mPtr->mDeviceContext.Get(); }
		template<> [[nodiscard]] static re12::detail::ShaderCompiler* Get() { return mPtr->mShaderCompiler.Get(); }
		template<> [[nodiscard]] static re12::detail::Keyboard* Get() { return mPtr->mKeyboard.Get(); }
		template<> [[nodiscard]] static re12::detail::Mouse* Get() { return mPtr->mMouse.Get(); }
	};
}