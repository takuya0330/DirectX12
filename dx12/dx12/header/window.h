#pragma once
#include <Windows.h>

class Window
{
public:
	Window() = default;
	~Window() = default;
public:
	void Initialize();
	bool Update();
	void Show(int _cmdshow);
public:
	HWND GetHwnd() { return hwnd; }
private:
	HWND hwnd;
};