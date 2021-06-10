#include "FrontendSprite.h"

void FrontendSprite::Render()
{
	for (int i = 0; i < DATA_MAX; ++i)
	{
		if (!mData[i].isEnable) continue;
		mData[i].tex->Render(mData[i].pos, mData[i].scale, mData[i].texPos, mData[i].size, mData[i].center);
		mData[i].isEnable = false;
		mData[i].tex = nullptr;
	}
}

bool FrontendSprite::Set(const std::shared_ptr<Sprite>& tex, const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center)
{
	for (int i = 0; i < DATA_MAX; ++i)
	{
		// —LŒø‚È‚ç–³Ž‹
		if (mData[i].isEnable) continue;
		
		mData[i].tex = tex;
		mData[i].pos = pos;
		mData[i].scale = scale;
		mData[i].texPos = texPos;
		mData[i].size = size;
		mData[i].center = center;
		mData[i].isEnable = true;

		return true;
	}

	return false;
}
