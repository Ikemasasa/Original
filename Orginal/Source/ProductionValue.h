#pragma once
#include <vector>

#include "lib/Font.h"

#include "Define.h"

class ProductionValue
{
	static const int FONT_SIZE = 46;
	static const int FONT_WEIGHT = 46;

	static constexpr float ALPHA_ADD = 0.15f;
	static constexpr float SIN_FACTOR_INIT = Define::PI * 0.2f;
	static constexpr float SIN_FACTOR_ADD = Define::PI * 0.1f;
	static constexpr float DISP_TIME = 1.2f;

	struct DelayData
	{
		int value;
		Vector3 pos;
		Vector3 rgb;
		int delayFrame;
		int elapsedFrame;
	};
	std::vector<DelayData> mDelayData;

	struct ValueData
	{
		int value;
		float timer;
		Vector3 pos;
		Vector4 color;
		int alphaSign;
		float sinfactor;
	};
	std::vector<ValueData> mValueData;

	Font mFont;

public:
	ProductionValue();
	~ProductionValue();

	void Add(const int& value, const Vector3& pos, const Vector3& rgb);
	void DelayAdd(const int& value, const Vector3& pos, const Vector3& rgb, const int delayFrame = 3);

	void Initialize();
	void Update();
	void Render();

	int GetFontSize() const { return mFont.GetFontSize(); }
	bool IsProcessing() const { return (!mValueData.empty() || !mDelayData.empty()); }
};