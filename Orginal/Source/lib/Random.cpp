#include "Random.h"

#include <random>

using namespace std;

int Random::Rand()
{
	random_device rnd;
	std::mt19937 mt(rnd());

	return mt();
}

int Random::RandomRange(const int& min, const int& max)
{
	random_device rnd;
	std::mt19937 mt(rnd());

	// min ~ maxの値を等確立で発生させる
	uniform_int_distribution<> range(min, max);
	return range(mt);
}

float Random::RandomRangef(const float& min, const float& max)
{
	random_device rnd;
	std::mt19937 mt(rnd());

	// min ~ maxの値を等確立で発生させる
	uniform_real_distribution<float> range(min, max);
	return range(mt);
}
