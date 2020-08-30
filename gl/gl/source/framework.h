#pragma once
#include <Windows.h>

#include "noncopyable.h"

class framework : private noncopyable
{
private:
	HWND hwnd_;

public:
	framework() = default;
	~framework() = default;

	int run(WNDPROC _wndproc, HINSTANCE _hinstance, int _cmdshow, UINT _width, UINT _height);
};