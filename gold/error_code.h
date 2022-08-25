#pragma once

enum class error_code
{
	success                           = 0,
	stopped                           = -1,
	graphics_init_windowcreate_failed = -2,
	graphics_init_graphics_failed     = -3,
	graphics_run_failed               = -4,
	already_exists                    = -5,
	already_run                       = -6,
	run_begin_first                   = -7,
	width_too_big                     = -8,
	height_too_big                    = -9
};