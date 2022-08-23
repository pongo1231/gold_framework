#include "input.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void gold_input::run()
{
	for (BYTE idx = 0; idx < sizeof(key_states) / sizeof(key_states[0]); idx++)
	{
		key_prev_states[idx] = key_states[idx];
		key_states[idx]      = GetAsyncKeyState(idx);
	}

	ShowCursor(FALSE);
}

bool gold_input::is_key_pressed(BYTE key_code) const
{
	return key_states[key_code];
}

bool gold_input::is_key_just_pressed(BYTE key_code) const
{
	return key_states[key_code] && !key_prev_states[key_code];
}