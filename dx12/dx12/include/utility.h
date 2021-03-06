#pragma once
#include <Windows.h>
#include <iostream>

struct Debug
{
	inline static void Console(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vprintf(format, ap);
		va_end(ap);
	}

	inline static void Console(const wchar_t* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vwprintf(format, ap);
		va_end(ap);
	}
};

struct ID3D12Resource;
bool WriteUploadHeapMemory(ID3D12Resource* _resource, const void* _data, uint32_t _size);