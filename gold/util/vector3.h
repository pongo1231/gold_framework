#pragma once

#include <cstdint>

class gold_vector3
{
  public:
	float x = 0.f, y = 0.f, z = 0.f;

	gold_vector3() = default;

	gold_vector3(float x, float y, float z) : x(x), y(y), z(z)
	{
	}

	float &operator[](std::uint8_t index)
	{
		return *(&x + index);
	}

	gold_vector3 operator+(const gold_vector3 &vector) const
	{
		gold_vector3 new_vector;

		new_vector.x = x + vector.x;
		new_vector.y = y + vector.y;
		new_vector.z = z + vector.z;

		return new_vector;
	}

	void operator+=(const gold_vector3 &vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
	}

	gold_vector3 operator+(float value) const
	{
		gold_vector3 new_vector;

		new_vector.x = x + value;
		new_vector.y = y + value;
		new_vector.z = z + value;

		return new_vector;
	}

	void operator+=(float value)
	{
		x += value;
		y += value;
		z += value;
	}

	gold_vector3 operator-(const gold_vector3 &vector) const
	{
		gold_vector3 new_vector;

		new_vector.x = x - vector.x;
		new_vector.y = y - vector.y;
		new_vector.z = z - vector.z;

		return new_vector;
	}

	void operator-=(const gold_vector3 &vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
	}

	gold_vector3 operator-(float value) const
	{
		gold_vector3 new_vector;

		new_vector.x = x - value;
		new_vector.y = y - value;
		new_vector.z = z - value;

		return new_vector;
	}

	void operator-=(float value)
	{
		x -= value;
		y -= value;
		z -= value;
	}

	gold_vector3 operator*(const gold_vector3 &vector) const
	{
		gold_vector3 new_vector;

		new_vector.x = x * vector.x;
		new_vector.y = y * vector.y;
		new_vector.z = z * vector.z;

		return new_vector;
	}

	void operator*=(const gold_vector3 &vector)
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
	}

	gold_vector3 operator*(float value) const
	{
		gold_vector3 new_vector;

		new_vector.x = x * value;
		new_vector.y = y * value;
		new_vector.z = z * value;

		return new_vector;
	}

	void operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;
	}

	gold_vector3 normalize() const
	{
		gold_vector3 vec;

		auto total = x + y + z;
		vec.x      = x / total;
		vec.y      = y / total;
		vec.z      = z / total;

		return vec;
	}

	gold_vector3 cross(const gold_vector3 &vector) const
	{
		gold_vector3 new_vector;

		new_vector.x = y * vector.z - z * vector.y;
		new_vector.y = z * vector.x - x * vector.z;
		new_vector.z = x * vector.y - y * vector.x;

		return new_vector;
	}

	float dot(const gold_vector3 &vector) const
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}
};