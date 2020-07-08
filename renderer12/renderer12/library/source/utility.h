#pragma once
#include <Windows.h>
#include <iostream>

namespace utility
{
	inline void Print(const char* format, ...)
	{
#if _DEBUG
		va_list valist;
		va_start(valist, format);
		vprintf(format, valist);
		va_end(valist);
#endif
	}

	inline LPWSTR HRTrace(HRESULT hr)
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
}

#if _DEBUG
#define ASSERTION(expr, msg) \
	(void)((!!(expr)) || \
	(1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, "%s", msg)) || \
	(_CrtDbgBreak(), 0))
#else
#define ASSERTION(expr, expr_str) ((void)0)
#endif