#pragma once
#include <Windows.h>

#include "property.h"

class Window
{
public:
	Window() = default;
	~Window() = default;

	void Initialize(const char* _title, UINT _width, UINT _height);
	bool Update();

	Property<HWND> Hwnd{ [this]() { return hwnd_; }, nullptr };
	Property<UINT> Width{ [this]() { return width_; }, [this](UINT _value) { width_ = _value; } };
	Property<UINT> Height{ [this]() { return height_; }, [this](UINT _value) { height_ = _value; } };
private:
	HWND hwnd_ = {};
	UINT width_ = 0;
	UINT height_ = 0;
};