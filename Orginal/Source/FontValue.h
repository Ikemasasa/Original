#pragma once
#include <vector>

#include "lib/Vector.h"
#include "lib/Font.h"

// �����͂悭�g�����A�����ɂ߂�ǂ���������N���X�����Ă݂�

class FontValue
{
	static const int FONT_SIZE = 64;
	static const int FONT_WEIGHT = 64;

	Font mFont;

public:
	FontValue();
	~FontValue();

	void RenderSet(const int value, const Vector3& pos, const Vector2& scale, const Vector2& centerPercent, const Vector4& color);
	void RenderSet(const int value, const Vector2& pos, const Vector2& scale, const Vector2& centerPercent, const Vector4& color);
	void Render();
};