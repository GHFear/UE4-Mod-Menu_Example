#pragma once
#include <windows.h>
#include <cstdint>

inline HANDLE _hProcess;
#pragma once
#include "defs.h"
#include <windows.h>

extern uint64 Base;

bool Read(void* address, void* buffer, uint64 size);
template <typename T> T Read(void* address) {
	T buffer{};
	Read(address, &buffer, sizeof(T));
	return buffer;
}

bool Write(void* address, void* buffer, uint64 size);
template <typename T> T Write(void* address) {
	T buffer{};
	Write(address, &buffer, sizeof(T));
	return buffer;
}

bool ReaderInit(uint32 pid);
