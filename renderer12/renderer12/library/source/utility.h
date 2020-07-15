#pragma once
#include <Windows.h>
#include <iostream>

namespace snd::utility
{
#if _DEBUG
	inline void Print(const char* _format, ...)
	{
		char str[1024];
		sprintf_s(str, _format);
		OutputDebugStringA(str);
		OutputDebugStringA("\n");
	}

	inline void Print(const wchar_t* _format, ...)
	{
		wchar_t str[1024];
		swprintf_s(str, _format);
		OutputDebugStringW(str);
		OutputDebugStringW(L"\n");
	}
#else
	inline void Print(const char* _format, ...) 
	{
	}

	inline void Print(const wchar_t* _format, ...)
	{
	}
#endif
	template<class T>
	inline void SafeDelete(T* _object)
	{
		if (_object) { delete _object; _object = nullptr; }
	}
}

#if _DEBUG
#define MY_ASSERT(expr, msg) (void)((!!(expr)) || (1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, "%s", msg)) || (_CrtDbgBreak(), 0))
#define ASSERT_MESSAGE(hr, msg) MY_ASSERT(SUCCEEDED(hr), msg)
#else
#define MY_ASSERT(expr, msg) ((void)0)
#define ASSERT_MESSAGE(hr, msg) ((void)0)
#endif