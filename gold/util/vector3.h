#pragma once

#define _NODISCARD [[nodiscard]]

class gold_vector3
{
  public:
	float x = 0.f, y = 0.f, z = 0.f;

	gold_vector3() = default;

	inline gold_vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{
	}

	_NODISCARD inline gold_vector3 operator+(const gold_vector3 &vec) const
	{
		gold_vector3 new_vec;

		new_vec.x = x + vec.x;
		new_vec.y = y + vec.y;
		new_vec.z = z + vec.z;

		return new_vec;
	}

	inline void operator+=(const gold_vector3 &vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	_NODISCARD inline gold_vector3 operator-(const gold_vector3 &vec) const
	{
		gold_vector3 new_vec;

		new_vec.x = x - vec.x;
		new_vec.y = y - vec.y;
		new_vec.z = z - vec.z;

		return new_vec;
	}

	inline void operator-=(const gold_vector3 &vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	_NODISCARD inline gold_vector3 operator*(const gold_vector3 &vec) const
	{
		gold_vector3 new_vec;

		new_vec.x = x * vec.x;
		new_vec.y = y * vec.y;
		new_vec.z = z * vec.z;

		return new_vec;
	}

	inline void operator*=(const gold_vector3 &vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}

	_NODISCARD inline gold_vector3 norm() const
	{
		gold_vector3 vec;

		auto total = x + y + z;
		vec.x      = x / total;
		vec.y      = y / total;
		vec.z      = z / total;

		return vec;
	}

	_NODISCARD inline gold_vector3 cross(const gold_vector3 &vec) const
	{
		gold_vector3 new_vec;

		new_vec.x = y * vec.z - z * vec.y;
		new_vec.y = z * vec.x - x * vec.z;
		new_vec.z = x * vec.y - y * vec.x;

		return new_vec;
	}

	_NODISCARD inline float dot(const gold_vector3 &vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}
};