#pragma once
#include "global.h"
#include <iostream>
#include <fstream>


#undef max
#define M_PI 3.14159265358979323846f

namespace Util
{



	uintptr_t FindPattern(uintptr_t start, size_t length, const unsigned char* pattern, const char* mask)
	{
		size_t pos = 0;
		auto maskLength = std::strlen(mask) - 1;

		auto startAdress = start;
		for (auto it = startAdress; it < startAdress + length; ++it)
		{
			if (*reinterpret_cast<unsigned char*>(it) == pattern[pos] || mask[pos] == '?')
			{
				if (mask[pos + 1] == '\0')
				{
					return it - maskLength;
				}

				pos++;
			}
			else
			{
				pos = 0;
			}
		}

		return -1;
	}
	uintptr_t StartPattern(HMODULE module, const unsigned char* pattern, const char* mask)
	{
		MODULEINFO info = { };
		GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(MODULEINFO));

		return FindPattern(reinterpret_cast<uintptr_t>(module), info.SizeOfImage, pattern, mask);
	}


}