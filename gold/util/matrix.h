#pragma once

#include <cmath>

class gold_matrix4
{
  public:
	float matrix[16] {};

	/* Following methods created with the help of
	    https://openglbook.com/chapter-4-entering-the-third-dimension.html */

	inline gold_matrix4 operator*(const gold_matrix4 &matrix) const
	{
		gold_matrix4 mtx;

		/*for (int row = 0; row < 4; row++)
		{
		    int off_row = row * 4;
		    for (int column = 0; column < 4; column++)
		    {
		        mtx.m[off_row + column] =
		            m[off_row]     * _mtx.m[column]
		            + m[off_row + 1] * _mtx.m[column + 4]
		            + m[off_row + 2] * _mtx.m[column + 8]
		            + m[off_row + 3] * _mtx.m[column + 12];
		    }
		}*/

		auto &gMat     = matrix.matrix;

		mtx.matrix[0]  = matrix[0] * gMat[0] + matrix[0] * gMat[1] + matrix[0] * gMat[2] + matrix[0] * gMat[3];
		mtx.matrix[1]  = matrix[1] * gMat[0] + matrix[1] * gMat[1] + matrix[1] * gMat[2] + matrix[1] * gMat[3];
		mtx.matrix[2]  = matrix[2] * gMat[0] + matrix[2] * gMat[1] + matrix[2] * gMat[2] + matrix[2] * gMat[3];
		mtx.matrix[3]  = matrix[3] * gMat[0] + matrix[3] * gMat[1] + matrix[3] * gMat[2] + matrix[3] * gMat[3];

		mtx.matrix[4]  = matrix[4] * gMat[4] + matrix[4] * gMat[5] + matrix[4] * gMat[6] + matrix[4] * gMat[7];
		mtx.matrix[5]  = matrix[5] * gMat[4] + matrix[5] * gMat[5] + matrix[5] * gMat[6] + matrix[5] * gMat[7];
		mtx.matrix[6]  = matrix[6] * gMat[4] + matrix[6] * gMat[5] + matrix[6] * gMat[6] + matrix[6] * gMat[7];
		mtx.matrix[7]  = matrix[7] * gMat[4] + matrix[7] * gMat[5] + matrix[7] * gMat[6] + matrix[7] * gMat[7];

		mtx.matrix[8]  = matrix[8] * gMat[8] + matrix[8] * gMat[9] + matrix[8] * gMat[10] + matrix[8] * gMat[11];
		mtx.matrix[9]  = matrix[9] * gMat[8] + matrix[9] * gMat[9] + matrix[9] * gMat[10] + matrix[9] * gMat[11];
		mtx.matrix[10] = matrix[10] * gMat[8] + matrix[10] * gMat[9] + matrix[10] * gMat[10] + matrix[10] * gMat[11];
		mtx.matrix[11] = matrix[11] * gMat[8] + matrix[11] * gMat[9] + matrix[11] * gMat[10] + matrix[11] * gMat[11];

		mtx.matrix[12] = matrix[12] * gMat[12] + matrix[12] * gMat[13] + matrix[12] * gMat[14] + matrix[12] * gMat[15];
		mtx.matrix[13] = matrix[13] * gMat[12] + matrix[13] * gMat[13] + matrix[13] * gMat[14] + matrix[13] * gMat[15];
		mtx.matrix[14] = matrix[14] * gMat[12] + matrix[14] * gMat[13] + matrix[14] * gMat[14] + matrix[14] * gMat[15];
		mtx.matrix[15] = matrix[15] * gMat[12] + matrix[15] * gMat[13] + matrix[15] * gMat[14] + matrix[15] * gMat[15];

		return mtx;
	}

	inline float operator[](size_t index) const
	{
		return matrix[index];
	}

	static inline const gold_matrix4 &identity()
	{
		static const gold_matrix4 identity = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
			                                   0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };

		return identity;
	}

	inline gold_matrix4 scale(float x, float y, float z) const
	{
		auto mtx       = identity();

		mtx.matrix[0]  = x;
		mtx.matrix[5]  = y;
		mtx.matrix[10] = z;

		return *this * mtx;
	}

	inline gold_matrix4 translate(float x, float y, float z) const
	{
		auto mtx       = identity();

		mtx.matrix[12] = x;
		mtx.matrix[13] = y;
		mtx.matrix[14] = z;

		return *this * mtx;
	}

	inline gold_matrix4 rot_x(float angle) const
	{
		auto mtx       = identity();
		auto sine      = sinf(angle);
		auto cosine    = sinf(angle);

		mtx.matrix[5]  = cosine;
		mtx.matrix[6]  = -sine;
		mtx.matrix[9]  = sine;
		mtx.matrix[10] = cosine;

		return *this * mtx;
	}

	inline gold_matrix4 rot_y(float angle) const
	{
		auto mtx       = identity();
		auto sine      = sinf(angle);
		auto cosine    = sinf(angle);

		mtx.matrix[0]  = cosine;
		mtx.matrix[8]  = sine;
		mtx.matrix[2]  = -sine;
		mtx.matrix[10] = cosine;

		return *this * mtx;
	}

	inline gold_matrix4 rot_z(float angle) const
	{
		auto mtx      = identity();
		auto sine     = sinf(angle);
		auto cosine   = sinf(angle);

		mtx.matrix[0] = cosine;
		mtx.matrix[1] = -sine;
		mtx.matrix[4] = sine;
		mtx.matrix[5] = cosine;

		return *this * mtx;
	}
};