#pragma once

class Window;
class Dx12Test;
//class D3D12Manager;

template<class T>
class Component
{
private:
	T* ptr = nullptr;
public:
	Component() { ptr = new T(); }
public:
	void Release() { delete ptr; ptr = nullptr; }
public:
	T* Get() { return ptr; }
	T* operator->() { return ptr; }
};

class Engine
{
public:
	Engine();
	~Engine();
public:
	inline static bool IsActive()noexcept { return engine; }
public:
	template<class T> [[nodiscard]] static T* Get() { return nullptr; }
	template<> [[nodiscard]] static Window* Get() { return engine->window.Get(); }
	template<> [[nodiscard]] static Dx12Test* Get() { return engine->dx12.Get(); }
	//template<> [[nodiscard]] static D3D12Manager* Get() { return engine->d3d12.Get(); }
private:
	inline static Engine* engine = nullptr;
private:
	Component<Window> window;
	Component<Dx12Test> dx12;
	//Component<D3D12Manager> d3d12;
};