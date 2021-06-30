#pragma once
#include <vector>

#include "FontValue.h"

class ProductionValue
{
	static const int FONT_SIZE = 64;
	static const int FONT_WEIGHT = 64;

	struct ValueData
	{
		int value;
		Vector3 pos;
		Vector2 scale;
		Vector2 center;
		Vector4 color;
		int alphaSign;
	};
	std::vector<ValueData> mValueData;

	FontValue mFontValue;

public:
	ProductionValue();
	~ProductionValue();

	void Add(const int& value, const Vector3& pos, const Vector3& rgb);

	void Initialize();
	void Update();
	void Render();
};