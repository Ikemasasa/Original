#pragma once
#include <vector>

// とりあえず全部メルセンヌツイスタで乱数を生成する

namespace Random
{
	int Rand();

	int RandomRange(const int& min, const int& max);
	float RandomRangef(const float& min, const float& max);
}