#include "Font.h"

#include "Framework.h"
#include "Math.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Sprite.h"

TCHAR Font::mTTFPath[Font::STR_MAX] = { '\0' };
WCHAR Font::mFontName[LF_FACESIZE] = L"ＭＳ Ｐゴシック";  // デフォルトのフォント

void Font::Initialize(int fontSize, int fontWeight)
{
	mFontSize = fontSize;
	mFontWeight = fontWeight;
}

void Font::Release()
{
	// 文字情報削除
	mFonts.clear();
}

bool Font::EnableTTF(const TCHAR* ttfPath, const WCHAR* fontname)
{
	// すでにほかのTTFを参照してたら false
	if (IsTTFEnabled()) return false;

	// TTF追加
	wcscpy_s(mTTFPath, STR_MAX, ttfPath);
	wcscpy_s(mFontName, LF_FACESIZE, fontname);
	AddFontResourceEx(mTTFPath, FR_PRIVATE, NULL);
	return true;
}

void Font::DisableTTF()
{
	// TTF削除
	if (IsTTFEnabled())
	{
		RemoveFontResourceEx(mTTFPath, FR_PRIVATE, NULL);
	}
}

bool Font::RenderSet(const wchar_t* str, const Vector2& pos, const Vector2& center, const Vector2& scale, const Vector4& color)
{
	RenderData data;

	for (size_t i = 0; i < mFonts.size(); ++i)
	{
		if (wcscmp(mFonts[i].str, str) != 0) continue;

		data.fontIndex = i;
		data.scrPos = pos;
		data.center = center;
		data.scale = scale;
		data.color = color;
		mRenderData.emplace_back(data);
		return true;
	}

	// なかったらつくる
	if (Add(str))
	{
		data.fontIndex = mFonts.size() - 1;
		data.scrPos = pos;
		data.center = center;
		data.scale = scale;
		data.color = color;
		mRenderData.emplace_back(data);
		return true;
	}

	return false;
}

bool Font::RenderSet(const int index, const Vector2& pos, const Vector2& center, const Vector2& scale, const Vector4& color)
{
	if (mFonts.size() < static_cast<size_t>(index)) return false;

	RenderData data;

	data.fontIndex = index;
	data.scrPos = pos;
	data.center = center;
	data.scale = scale;
	data.color = color;
	mRenderData.emplace_back(data);
	return true;
}

void Font::ClearRenderSet()
{
	mRenderData.clear();
}

void Font::Render(bool isRenderClear)
{
	Vector2 pos = {};
	Vector2 scale = {};
	Vector2 texPos = { 0.0f, 0.0f };
	Vector2 size = {};
	Vector2 center = {};
	float angle = 0;
	Vector4 color = {};

	auto ClampColor = [&color]()
	{
		color.x = Math::Clamp01(color.x);
		color.y = Math::Clamp01(color.y);
		color.z = Math::Clamp01(color.z);
		color.w = Math::Clamp01(color.w);
	};

	for (auto& data : mRenderData)
	{
		const FontData& font = mFonts[data.fontIndex];
		scale = data.scale;
		center = data.center;
		color = data.color;

		size_t num = font.textures.size();
		for (size_t i = 0; i < num; ++i)
		{
			pos = data.scrPos + font.pos[i];
			size = font.size[i];

			Renderer2D::GetInstance().Render(font.textures[i].GetSRV(), pos, scale, texPos, size, center, angle, color);
		}
	}

	if(isRenderClear) mRenderData.clear();
}

bool Font::Add(const wchar_t* str)
{
	size_t size = mFonts.size();
	mFonts.resize(size + 1);
	FontData& data = mFonts.back();

	wcscpy_s(data.str, STR_MAX, str);
	
	int len = wcslen(str);
	data.textures.resize(len);
	data.pos.resize(len);
	data.size.resize(len);

	GLYPHMETRICS gm;
	TEXTMETRIC tm;
	float offsetX = 0.0f;
	for (int i = 0; i < len; ++i)
	{
		data.textures[i].Create(str[i], mFontName, mFontSize, mFontWeight, &gm, &tm);

		// 座標計算
		data.pos[i].x = static_cast<float>(offsetX + gm.gmptGlyphOrigin.x);
		data.pos[i].y = static_cast<float>(tm.tmAscent - gm.gmptGlyphOrigin.y);
		offsetX += gm.gmCellIncX;

		// サイズ
		data.size[i].x = static_cast<float>(gm.gmBlackBoxX);
		data.size[i].y = static_cast<float>(gm.gmBlackBoxY);
	}
	data.width = offsetX;

	return true;
}

bool Font::Remove(const wchar_t* str)
{
	for (auto it = mFonts.begin(); it != mFonts.end(); ++it)
	{
		if (wcscmp(it->str, str) != 0) continue;

		mFonts.erase(it);
		return true;
	}

	return false;
}

bool Font::Find(const wchar_t* str)
{
	for(auto& font : mFonts)
	{
		if (wcscmp(font.str, str) != 0) continue;

		return true;
	}

	return false;
}

float Font::GetWidth(const WCHAR* str) 
{	
	// 同じ文字列があるかチェック
	size_t fontIndex = -1;
	for (size_t i = 0; i < mFonts.size(); ++i)
	{
		if (wcscmp(mFonts[i].str, str) != 0) continue;

		//あった
		fontIndex = i;
		break;
	}

	// 見つからなかったら作る
	if (fontIndex == -1)
	{
		if(Add(str))
			fontIndex = mFonts.size() - 1;
	}

	return GetWidth(fontIndex);
}

float Font::GetWidth(const UINT index) const
{
	return mFonts[index].width;
}