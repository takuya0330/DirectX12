#pragma once
#include <Windows.h>
#include <iostream>

namespace snd::utility
{
#if _DEBUG
	inline void print(const char* format, ...)
	{
		va_list valist;
		va_start(valist, format);
		vprintf(format, valist);
		va_end(valist);
	}
#else
	inline void print(const char* format, ...) {}
#endif

	inline LPWSTR whr_trace(HRESULT hr)
	{
		LPWSTR msg;
		FormatMessageW
		(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPWSTR>(&msg),
			0,
			NULL
		);
		return msg;
	}
	inline LPSTR hr_trace(HRESULT hr)
	{
		LPSTR msg;
		FormatMessageA
		(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&msg),
			0,
			NULL
		);
		return msg;
	}
}

#if _DEBUG
#define MY_ASSERT(expr, msg) (void)((!!(expr)) || (1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, "%s", msg)) || (_CrtDbgBreak(), 0))
#define ASSERT_SUCCEEDED(hr) MY_ASSERT(SUCCEEDED(hr), snd::utility::hr_trace(hr))
#define ASSERT_MESSAGE(hr, msg) MY_ASSERT(SUCCEEDED(hr), msg)
#else
#define MY_ASSERT(expr, expr_str) ((void)0)
#define ASSERT_SUCCEEDED(hr) ((void)0)
#define ASSERT_MESSAGE(hr, msg) ((void)0)
#endif