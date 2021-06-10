#pragma once

#include <memory>

#include "lib/Vector.h"

#include "lib/Sprite.h"

class FrontendSprite
{
	// よくよく考えるとこのクラスいるか・・・？と思ったけど、
	// 描画のタイミングとかうんぬんであると便利かもしれない
	// コストに見合うかは・・・どうだろう
	// もっといい方法はありそう

	static const int DATA_MAX = 64;

	struct Data
	{
		std::shared_ptr<Sprite> tex = nullptr;
		Vector2 pos					 = { 0.0f, 0.0f };
		Vector2 scale				 = { 0.0f, 0.0f };
		Vector2 texPos				 = { 0.0f, 0.0f };
		Vector2 size				 = { 0.0f, 0.0f };
		Vector2 center				 = { 0.0f, 0.0f };
		bool isEnable				 = false;
	};
	Data mData[DATA_MAX];

public:
	void Render();
	bool Set(const std::shared_ptr<Sprite>& tex, const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center);

};