#pragma once
#include <vector>
#include <map>

#include "Vector.h"
#include "FontTexture.h"

// 追々縁とかつくりたい

// static でひらがな、カタカナをあらかじめ用意して、使いまわせるようにすると便利・・・かも！
// なんなら数字も一緒にできるか・・・？ FontValueいらない説

class Font
{
	// static
private:
	static const int STR_MAX = 64;

	static TCHAR mTTFPath[STR_MAX];
	static WCHAR mFontName[LF_FACESIZE];

	static bool IsTTFEnabled() { return (mTTFPath[0] != '\0'); }

public:
	static bool EnableTTF(const TCHAR* ttfPath, const WCHAR* fontname);
	static void DisableTTF();
	// static 終わり

private:
	struct FontData
	{
		wchar_t str[STR_MAX];
		std::vector<FontTexture> textures;
		std::vector<Vector2> pos;
		std::vector<Vector2> size;

	};

	struct RenderData
	{
		int fontIndex;
		Vector2 scrPos;
		Vector2 scale;
		Vector2 center;
		Vector4 color;
	};

	std::vector<FontData> mFonts;
	std::vector<RenderData> mRenderData;
	int mFontSize = 0;
	int mFontWeight = 0;

public:
	Font() = default;
	~Font() = default;

	void Initialize(int fontSize, int fontWeight);
	void Release();

	bool RenderSet(const wchar_t* str, const Vector2& pos, const Vector2& center = Vector2::Zero(), const Vector2& scale = Vector2::One(), const Vector4& color = Vector4::One());
	bool RenderSet(const UINT index, const Vector2& pos, const Vector2& center = Vector2::Zero(), const Vector2& scale = Vector2::One(), const Vector4& color = Vector4::One());
	void Render();

	bool Add(const wchar_t* str);
	bool Find(const wchar_t* str);
	float GetWidth(const WCHAR* str);
	size_t GetNum() const { return mFonts.size(); }
};