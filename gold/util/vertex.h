#pragma once

/*
* Gold vertex struct
*/
struct gold_vertex
{
	// Position
	float pos_x = 0.f, pos_y = 0.f, pos_z = 0.f;
	// Texture UV
	float tex_u = 0.f, tex_v = 0.f;
	// Color RGBA
	float col_r = 0.f, col_g = 0.f, col_b = 0.f, col_a = 0.f;
	// Normal
	float norm_x = 0.f, norm_y = 0.f, norm_z = 0.f;
};