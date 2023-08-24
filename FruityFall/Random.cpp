#include "Random.h"

#include <random>

namespace
{
	std::random_device rd;

	std::mt19937 re{ rd() };
}

namespace Random
{
	int32_t int32(int32_t from, int32_t to)
	{
		std::uniform_int_distribution<int32_t> num(from, to);

		return num(re);
	}

	uint32_t uint32(uint32_t from, uint32_t to)
	{
		std::uniform_int_distribution<uint32_t> num(from, to);

		return num(re);
	}
}