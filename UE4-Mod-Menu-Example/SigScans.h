#pragma once
#include "util.h"


namespace SigScans
{

	// Find AddrAPBP_SessionPhysicalDayNight_C

	int Get_APBP_SessionPhysicalDayNight_C()
	{

		//Signature patter and mask
		char pattern[] = "\xA0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x28\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xF6\x3E\x05\x00\x03";
		const char* mask = "x?????????x?????????????xxx?x";

		//Run pattern scan
		auto AddrAPBP_SessionPhysicalDayNight_C = Util::StartPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>(pattern), mask);


		//Exit if we don't find a pattern
		if (AddrAPBP_SessionPhysicalDayNight_C == -1)
		{

			return -1;
		}

		return AddrAPBP_SessionPhysicalDayNight_C - (uintptr_t)GetModuleHandleW(nullptr);
	}



}