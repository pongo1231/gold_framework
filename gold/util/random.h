#pragma once

#include <random>

class gold_random
{
  private:
	std::random_device random_device;
	std::mt19937 random { random_device() };

  public:
	inline void set_seed(int seed)
	{
		random.seed(seed);
	}

	inline int random_int(int lower, int upper)
	{
		std::uniform_int_distribution<> distr(lower, upper);
		return distr(random);
	}

	inline float random_float(float lower, float upper)
	{
		std::uniform_real_distribution<float> distr(lower, upper);
		return distr(random);
	}
};

inline gold_random gold_random;