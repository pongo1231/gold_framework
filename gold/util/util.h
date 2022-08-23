#pragma once

#include "gold/util/matrix.h"
#include "gold/util/vector3.h"

#include <string_view>

namespace gold_util
{
	inline gold_matrix4 look_at(const gold_vector3 &eye, const gold_vector3 &at, const gold_vector3 &up)
	{
		gold_vector3 forward;
		gold_vector3 side;
		gold_vector3 _up;

		forward          = (at - eye).norm();
		side             = forward.cross(up).norm();
		_up              = side.cross(forward);

		gold_matrix4 mtx = { side.x, _up.x, -forward.x, 0.f, side.y, _up.y, -forward.y, 0.f,
			                 side.z, _up.z, -forward.z, 0.f, 0.f,    0.f,   0.f,        1.f };

		mtx.matrix[12]   = mtx.matrix[0] * -eye.x + mtx.matrix[4] * -eye.y + mtx.matrix[8] * -eye.z + mtx.matrix[12];
		mtx.matrix[13]   = mtx.matrix[1] * -eye.x + mtx.matrix[5] * -eye.y + mtx.matrix[9] * -eye.z + mtx.matrix[13];
		mtx.matrix[14]   = mtx.matrix[2] * -eye.x + mtx.matrix[6] * -eye.y + mtx.matrix[10] * -eye.z + mtx.matrix[14];
		mtx.matrix[15]   = mtx.matrix[3] * -eye.x + mtx.matrix[7] * -eye.y + mtx.matrix[11] * -eye.z + mtx.matrix[15];

		return mtx;

		/*auto z_axis = (_eye - _at).norm();
		auto x_axis = _up.cross(z_axis).norm();
		auto y_axis = z_axis.cross(x_axis);

		gol_mtx4 mtx =
		{
		    x_axis.x,  x_axis.y,  x_axis.z, -x_axis.dot(_eye),
		    y_axis.x,  y_axis.y,  y_axis.z, -y_axis.dot(_eye),
		    -z_axis.x, -z_axis.y, -z_axis.z,  z_axis.dot(_eye),
		    0.f,       0.f,       0.f,       1.f
		};*/
		/*gol_mtx4 mtx =
		{
		    x_axis.x,  y_axis.x,  z_axis.x, 0.f,
		    x_axis.y,  y_axis.y,  z_axis.y, 0.f,
		    x_axis.z,  y_axis.z,  z_axis.z, 0.f,
		    -(x_axis).dot(_eye),       -(y_axis).dot(_eye),       -(z_axis).dot(_eye),       1.f
		};

		return mtx;*/

		/*auto fwd = (_at - _eye).norm();
		auto side = fwd.cross(_up).norm();
		auto up = side.cross(fwd);

		gol_mtx4 mtx =
		{
		    side.x, up.x, -fwd.x, 0.f,
		    side.y, up.y, -fwd.y, 0.f,
		    side.z, up.z, -fwd.z, 0.f,
		    0.f, 0.f, 0.f, 1.f
		};
		mtx = mtx.translate(-_eye.x, -_eye.y, -_eye.z);

		return mtx;*/
	}

	// Taken from https://openglbook.com/chapter-4-entering-the-third-dimension.html
	inline gold_matrix4 create_proj_matrix(float fov_y, float aspect, float n_plane, float f_plane)
	{
		auto cotf = [](float _f)
		{
			return cosf(_f) / sinf(_f);
		};

		auto d2r         = 3.141592f / 180.f;
		float y_scale    = 1.0 / tanf(d2r * fov_y / 2);
		float x_scale    = y_scale / aspect;
		auto nearmfar    = n_plane - f_plane;

		gold_matrix4 mtx = { x_scale,
			                 0,
			                 0,
			                 0,
			                 0,
			                 y_scale,
			                 0,
			                 0,
			                 0,
			                 0,
			                 (f_plane + n_plane) / nearmfar,
			                 -1,
			                 0,
			                 0,
			                 2 * f_plane * n_plane / nearmfar,
			                 0 };

		return mtx;
	}
}