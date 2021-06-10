#include "FrontendBattle.h"

#include "lib/Font.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "CommandPlayer.h" 

FrontendBattle::FrontendBattle()
{
}

void FrontendBattle::Update()
{
	for (int i = 0; i < VALUE_DATA_MAX; ++i)
	{
		if (!mValueData[i].isEnable) continue;

		const float ADD_ALPHA = 0.04f;
		mValueData[i].color.w += ADD_ALPHA * mValueData[i].sign;
		if (mValueData[i].color.w >= 2.0f) mValueData[i].sign = -1;

		if (mValueData[i].color.w > 0.0f)
		{
			// ç¿ïWÇÇøÇÂÇ¡Ç∆Ç∏Ç¬è„Ç…à⁄ìÆÇ≥ÇπÇÈ
			constexpr float addPos = 0.01f;
			mValueData[i].pos.y += addPos;
			mValue.RenderSet(mValueData[i].value, mValueData[i].pos, Vector2::One(), mValueData[i].centerPercent, mValueData[i].color);
		}
		else
		{
			mValueData[i].isEnable = false;
		}
	}
}

void FrontendBattle::Render()
{
	mSprite.Render();
	mValue.Render();
}

bool FrontendBattle::SetValue(const int value, const Vector3& pos, const Vector2& centerPercent, const Vector4& color)
{
	for (int i = 0; i < VALUE_DATA_MAX; ++i)
	{
		if (mValueData[i].isEnable) continue;

		mValueData[i].value = value;
		mValueData[i].pos = pos;
		mValueData[i].centerPercent = centerPercent;
		mValueData[i].color = color;
		mValueData[i].sign = 1;
		mValueData[i].isEnable = true;

		return true;
	}

	return false;
}

void FrontendBattle::SetSprite(const std::shared_ptr<Sprite>& tex, const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center)
{
	mSprite.Set(tex, pos, scale, texPos, size, center);
}
