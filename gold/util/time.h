#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdint>

/*
* Timestamp of last frame
*/
inline std::uint64_t gold_last_frame_time = GetTickCount64();
/*
* Time since last frame in seconds
*/
inline float gold_delta_time              = 0.f;