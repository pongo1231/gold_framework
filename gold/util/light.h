#pragma once

#include "gold/util/vector3.h"

/*
* Position of global light used for phong shading
*/
inline gold_vector3 gold_global_light_position = { 0.f, 20.f, 0.f };
/*
* Color of global light used for phong shading (rgb, 0.0 - 1.0)
*/
inline gold_vector3 gold_global_light_color    = { .5f, .5f, .6f };