#pragma once
#ifdef _DEBUG
#include <crtdbg.h>
#include <string>
#define RE12_OUTPUT_CONSOLE(fmt, ...) _CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, (fmt + std::string("\n")).c_str(), __VA_ARGS__);
#define RE12_ASSERT_ERROR(expr, msg) (void)((!!(expr)) || (1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, "%s", msg)) || (_CrtDbgBreak(), 0))
#else 
#define RE12_OUTPUT_CONSOLE(fmt,...)
#define RE12_ASSERT_ERROR(expr, msg) ((void)0)
#endif // DEBUG