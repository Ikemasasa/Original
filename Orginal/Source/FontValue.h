#pragma once
#include <string>
#include <vector>

#include "lib/Vector.h"
#include "lib/Font.h"

// 数字はよく使うし、微妙にめんどくさいからクラス化してみた

class FontValue
{
	Font mFont;

public:
	FontValue();
	~FontValue();

	void Initialize(int fontSize = 32, int fontWeight = 32);
	void RenderSet(const int value, const Vector3& pos, const Vector2& center = Vector2::Zero(), const Vector2& scale = Vector2::One(), const Vector4& color = Vector4::One());
	void RenderSet(const int value, const Vector2& pos, const Vector2& center = Vector2::Zero(), const Vector2& scale = Vector2::One(), const Vector4& color = Vector4::One());
	void Render(bool isRenderClear = true);
	void Release();

	float GetWidth(const int value) const { return mFont.GetWidth(std::to_wstring(value).c_str()); }
};