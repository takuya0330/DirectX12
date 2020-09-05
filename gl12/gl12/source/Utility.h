#pragma once
#include <Windows.h>
#include <crtdbg.h>
#include <string>

#if defined( DEBUG ) || defined( _DEBUG )
#define _ASSERT_EXPR_A(expr, msg) (void)((!!(expr)) || (1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, "%s", msg)) || (_CrtDbgBreak(), 0))
#else
#define _ASSERT_EXPR_A(expr, expr_str) ((void)0)
#endif

inline LPWSTR hrTrace(HRESULT hr)
{
	LPWSTR msg;
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&msg), 0, NULL);
	return msg;
}

#if _DEBUG
inline void Console(const char* _format, ...)
{
	char str[1024];
	sprintf_s(str, _format);
	OutputDebugStringA(str);
	OutputDebugStringA("\n");
}
inline void Console(const wchar_t* _format, ...)
{
	wchar_t str[1024];
	swprintf_s(str, _format);
	OutputDebugStringW(str);
	OutputDebugStringW(L"\n");
}
#else
inline void Console(const char* _format, ...) {}
inline void Console(const wchar_t* _format, ...) {}
#endif