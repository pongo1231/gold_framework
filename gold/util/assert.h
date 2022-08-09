#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string_view>

inline void gold_assert(std::string_view text)
{
	MessageBoxA(NULL, text.data(), "GOLD ASSERTION", MB_OK | MB_ICONERROR);
	ExitProcess(0);
}