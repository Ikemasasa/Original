#pragma once
#include <list>
#include <memory>

#include "lib/Vector.h"

#include "FrontendSprite.h"
#include "FontValue.h"
#include "Singleton.h"

class Sprite;

//class FrontendBattle : public SingletonManual<FrontendBattle>
//{
//	static const int VALUE_DATA_MAX = 16;
//
//	struct ValueData
//	{
//		int value;
//		Vector3 pos;
//		Vector2 centerPercent;
//		Vector4 color;
//		int sign = 1;
//		bool isEnable = false;
//	};
//
//	ValueData mValueData[VALUE_DATA_MAX];
//
//	FrontendSprite mSprite;
//	FontValue mValue;
//
//public:
//	FrontendBattle();
//	~FrontendBattle() = default;
//
//	void Update();
//	void Render();
//
//	bool SetValue(const int value, const Vector3& pos, const Vector2& centerPercent, const Vector4& color);
//	void SetSprite(const std::shared_ptr<Sprite>& tex, const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center = Vector2::Zero());
//};