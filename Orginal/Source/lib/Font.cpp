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
	mFonts.clear();
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


bool Font::RenderSet(const wchar_t word, const Vector2& pos, const Vector2& centerRate, const Vector4& color)
{
	Add(word);

	RenderData rd;
	rd.str = word;
	rd.scrPos = pos;
	rd.centerRate = centerRate;
	rd.color = color;
	mRenderData.emplace_back(rd);

	return true;
}

bool Font::RenderSet(const wchar_t* str, const Vector2& pos, const Vector2& centerRate, const Vector4& color)
{
	Add(str);

	RenderData rd;
	rd.str = str;
	rd.scrPos = pos;
	rd.centerRate = centerRate;
	rd.color = color;

	mRenderData.emplace_back(rd);
	return true;
}

bool Font::RenderSetValue(const int value, const Vector2& pos, const Vector2& centerRate, const Vector4& color)
{
	std::wstring valueStr = std::to_wstring(value);
	Add(valueStr.c_str());

	RenderData rd;
	rd.str = valueStr.c_str();
	rd.scrPos = pos;
	rd.centerRate = centerRate;
	rd.color = color;
	mRenderData.emplace_back(rd);

	return true;
}

bool Font::RenderSetValue(const float value, const Vector2& pos, const Vector2& centerRate, const Vector4& color)
{
	std::wstring valueStr = std::to_wstring(value);
	Add(valueStr.c_str());

	RenderData rd;
	rd.str = valueStr.c_str();
	rd.scrPos = pos;
	rd.centerRate = centerRate;
	rd.color = color;
	mRenderData.emplace_back(rd);

	return true;
}

void Font::Render(bool isRenderClear)
{
	Vector2 pos;
	Vector2 scale(Vector2::ONE);
	Vector2 texPos(Vector2::ZERO);
	Vector2 center;
	Vector2 centerRate;
	float angle = 0.0f;
	Vector4 color;

	auto ClampColor = [&color]()
	{
		color.x = Math::Clamp01(color.x);
		color.y = Math::Clamp01(color.y);
		color.z = Math::Clamp01(color.z);
		color.w = Math::Clamp01(color.w);
	};

	for (const auto& rd : mRenderData)
	{
		pos = rd.scrPos;
		center = Vector2::ZERO;
		centerRate = rd.centerRate;
		color = rd.color;
		ClampColor();

		if(centerRate.x != 0.0f) center.x = GetWidth(rd.str.c_str()) * centerRate.x;
		for (size_t i = 0; i < rd.str.size(); ++i)
		{
			const FontData& fd = mFonts[rd.str[i]];
			pos.x += fd.leftTop.x;
			pos.y = rd.scrPos.y + fd.leftTop.y;
			Renderer2D::GetInstance().Render(fd.tex.GetSRV(), pos, scale, texPos, fd.size, center, angle, color);

			pos.x += fd.width - fd.leftTop.x;
		}
	}

	if (isRenderClear) mRenderData.clear();
}

bool Font::Add(const wchar_t* str)
{
	int len = wcslen(str);
	for (int i = 0; i < len; ++i)
	{
		// FontData取得
		auto& fd = mFonts[str[i]];

		// すでにあるならcontinue
		if (fd.tex.GetSRV()) continue;

		GLYPHMETRICS gm;
		TEXTMETRIC tm;

		// テクスチャ作成
		fd.tex.Create(str[i], mFontName, mFontSize, mFontWeight, &gm, &tm);

		// 左上座標取得
		fd.leftTop.x = gm.gmptGlyphOrigin.x;
		fd.leftTop.y = tm.tmAscent - gm.gmptGlyphOrigin.y;

		// サイズ取得
		fd.size.x = static_cast<float>(gm.gmBlackBoxX);
		fd.size.y = static_cast<float>(gm.gmBlackBoxY);

		// 横幅取得
		fd.width = gm.gmCellIncX;
	}

	return true;
}

bool Font::Add(wchar_t word)
{
	// FontData取得
	auto& fd = mFonts[word];

	// すでにあるならreturn
	if (fd.tex.GetSRV()) return true;

	GLYPHMETRICS gm;
	TEXTMETRIC tm;

	// テクスチャ作成
	fd.tex.Create(word, mFontName, mFontSize, mFontWeight, &gm, &tm);

	// 左上座標取得
	fd.leftTop.x = gm.gmptGlyphOrigin.x;
	fd.leftTop.y = tm.tmAscent - gm.gmptGlyphOrigin.y;

	// サイズ取得
	fd.size.x = static_cast<float>(gm.gmBlackBoxX);
	fd.size.y = static_cast<float>(gm.gmBlackBoxY);

	// 横幅取得
	fd.width = gm.gmCellIncX;

	return true;
}

bool Font::Remove(const wchar_t* str)
{
	int len = wcslen(str);
	for (int i = 0; i < len; ++i)
	{
		// ないならcontinue
		if (!mFonts[str[i]].tex.GetSRV()) continue;
		
		// 削除
		mFonts.erase(str[i]);
	}

	return true;
}

float Font::GetWidth(const WCHAR* str)
{
	Add(str);

	float ret = 0.0f;
	int len = wcslen(str);
	for (int i = 0; i < len; ++i)
	{
		ret += mFonts[str[i]].width;
	}

	return ret;
}

float Font::GetWidth(wchar_t word)
{
	Add(word);
	return mFonts[word].width;
}
