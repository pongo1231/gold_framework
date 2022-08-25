#pragma once

#include <random>

/*
* Random helper
*/
class gold_random
{
  private:
	std::random_device random_device;
	std::mt19937 random { random_device() };

  public:
	/*
	* Set the seed used for random generator
	* <param name="seed">Seed to use</param>
	*/
	inline void set_seed(int seed)
	{
		random.seed(seed);
	}

	/*
	* <param name="lower">Lower value (included)</param>
	* <param name="upper">Upper value (included)</param>
	* <returns>Random integer</returns>
	*/
	inline int random_int(int lower, int upper)
	{
		std::uniform_int_distribution<> distr(lower, upper);
		return distr(random);
	}

	/*
	 * <param name="lower">Lower value (included)</param>
	 * <param name="upper">Upper value (included)</param>
	 * <returns>Random float</returns>
	 */
	inline float random_float(float lower, float upper)
	{
		std::uniform_real_distribution<float> distr(lower, upper);
		return distr(random);
	}
};

inline gold_random gold_random;