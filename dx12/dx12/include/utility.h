#pragma once
#include <Windows.h>
#include <iostream>

#define TO_STRING(x) #x
#define TO_STRING_EX(x) TO_STRING(x)
#define OUTPUT_FILE TO_STRING_EX(__FILE__)
#define OUTPUT_LINE TO_STRING_EX(__LINE__)

namespace debug
{
	inline void Console(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vprintf(format, ap);
		va_end(ap);
	}

	inline void Console(const wchar_t* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vwprintf(format, ap);
		va_end(ap);
	}
}
