#include "Font.h"

#include "Framework.h"
#include "Math.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Texture.h"

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
	size_t sizeNum = mRenderData.size();
	mRenderData.resize(sizeNum + 1);

	for (int i = 0; i < mFonts.size(); ++i)
	{
		if (wcscmp(mFonts[i].str, str) != 0) continue;

		mRenderData[sizeNum].fontIndex = i;
		mRenderData[sizeNum].scrPos = pos;
		mRenderData[sizeNum].center = center;
		mRenderData[sizeNum].scale = scale;
		mRenderData[sizeNum].color = color;
		return true;
	}

	return false;
}

bool Font::RenderSet(const UINT index, const Vector2& pos, const Vector2& center, const Vector2& scale, const Vector4& color)
{
	size_t sizeNum = mRenderData.size();
	if (sizeNum < index) return false;

	mRenderData.resize(sizeNum + 1);

	mRenderData[sizeNum].fontIndex = index;
	mRenderData[sizeNum].scrPos = pos;
	mRenderData[sizeNum].center = center;
	mRenderData[sizeNum].scale = scale;
	mRenderData[sizeNum].color = color;
	return true;
}

void Font::Render()
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
		for (int i = 0; i < num; ++i)
		{
			pos = data.scrPos + font.pos[i];
			size = font.size[i];

			Renderer2D::GetInstance().Render(font.textures[i].GetSRV(), pos, scale, texPos, size, center, angle, color);
		}
	}

	mRenderData.clear();
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
		data.pos[i].x = offsetX + gm.gmptGlyphOrigin.x;
		data.pos[i].y = tm.tmAscent - gm.gmptGlyphOrigin.y;
		offsetX += gm.gmCellIncX;

		// サイズ
		data.size[i].x = static_cast<float>(gm.gmBlackBoxX);
		data.size[i].y = static_cast<float>(gm.gmBlackBoxY);
	}

	return true;
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
	HDC hdc = GetDC(NULL);

	// フォントの生成
	LOGFONT lf = { mFontSize, 0, 0, 0, mFontWeight, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, TEXT("\0") };
	wcscpy_s(lf.lfFaceName, LF_FACESIZE, mFontName);

	HFONT hFont = CreateFontIndirect(&lf);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// 横幅計算
	CONST MAT2 mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
	GLYPHMETRICS gm;
	float width = 0.0f;
	for (int i = 0; str[i] != '\0'; ++i)
	{
		GetGlyphOutline(hdc, (UINT)str[i], GGO_GRAY4_BITMAP, &gm, 0, NULL, &mat);
		width += static_cast<float>(gm.gmCellIncX);
	}

	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	return width;
}