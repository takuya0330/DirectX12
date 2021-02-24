#pragma once
#include <Windows.h>
#include <crtdbg.h>
#include <string>

#if _DEBUG
inline LPWSTR hResultTrace(HRESULT hr)
{
	LPWSTR msg;
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&msg), 0, NULL);
	return msg;
}

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

#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)

#define ASSERT_SUCCEEDED(hr) _ASSERT_EXPR(SUCCEEDED(hr), hResultTrace(hr))

#else
inline LPWSTR hResultTrace(HRESULT hr) 
{
}

inline void Console(const char* _format, ...) 
{
}

inline void Console(const wchar_t* _format, ...) 
{
}

#define ASSERT_SUCCEEDED(hr) ((void)0)

#endif