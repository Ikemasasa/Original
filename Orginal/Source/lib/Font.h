#pragma once
#include <map>
#include <string>
#include <vector>

#include "Vector.h"
#include "FontTexture.h"

// �ǁX���Ƃ����肽��

class Font
{
	friend class FontValue;

	// static
private:
	static const int STR_MAX = 128;

	static TCHAR mTTFPath[STR_MAX];
	static WCHAR mFontName[LF_FACESIZE];

	static bool IsTTFEnabled() { return (mTTFPath[0] != '\0'); }

public:
	static bool EnableTTF(const TCHAR* ttfPath, const WCHAR* fontname);
	static void DisableTTF();
	// static �I���

private:
	struct FontData
	{
		FontTexture tex;
		Vector2 leftTop;
		Vector2 size;
		float width;
	};

	struct RenderData
	{
		std::wstring str;
		Vector2 scrPos;
		Vector2 centerRate;
		Vector4 color;
	};

	std::map<wchar_t, FontData> mFonts;
	std::vector<RenderData> mRenderData;
	int mFontSize = 0;
	int mFontWeight = 0;

	// �t�H���g�ǉ�
	bool Add(const wchar_t* str);	
	bool Add(wchar_t word);
public:
	Font() = default;
	~Font() = default;

	void Initialize(int fontSize = 32, int fontWeight = 32);
	void Release();

	bool RenderSet(const wchar_t word, const Vector2& pos, const Vector2& center = Vector2::ZERO, const Vector4& color = Vector4::ONE);
	bool RenderSet(const wchar_t* str, const Vector2& pos, const Vector2& centerRate = Vector2::ZERO, const Vector4& color = Vector4::ONE);
	bool RenderSetValue(const int value, const Vector2& pos, const Vector2& centerRate = Vector2::ZERO, const Vector4& color = Vector4::ONE);
	bool RenderSetValue(const float value, const Vector2& pos, const Vector2& centerRate = Vector2::ZERO, const Vector4& color = Vector4::ONE);
	void Render(bool isRenderClear = true);


	bool Remove(const wchar_t* str); // �t�H���g�폜

	// �Q�b�^�[
	float GetWidth(const WCHAR* str);
	float GetWidth(wchar_t word);
	int GetFontSize() const { return mFontSize; }
	size_t GetNum() const { return mFonts.size(); }
	
	// �Z�b�^�[
	void ClearRenderSet() { mRenderData.clear(); }
};