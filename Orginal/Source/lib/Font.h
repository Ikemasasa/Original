#pragma once
#include <vector>
#include <map>

#include "Vector.h"
#include "FontTexture.h"

// �ǁX���Ƃ����肽��

// static �łЂ炪�ȁA�J�^�J�i�����炩���ߗp�ӂ��āA�g���܂킹��悤�ɂ���ƕ֗��E�E�E�����I
// �Ȃ�Ȃ琔�����ꏏ�ɂł��邩�E�E�E�H FontValue����Ȃ���

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
	// static �I���

private:
	struct FontData
	{
		wchar_t str[STR_MAX];
		std::vector<FontTexture> textures;
		std::vector<Vector2> pos;
		std::vector<Vector2> size;
		float width;
	};

	struct RenderData
	{
		int fontIndex;
		Vector2 scrPos;
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

	void Initialize(int fontSize = 32, int fontWeight = 32);
	void Release();

	bool RenderSet(const wchar_t* str, const Vector2& pos, const Vector2& center = Vector2::ZERO, const Vector4& color = Vector4::ONE);
	bool RenderSet(const int index, const Vector2& pos, const Vector2& center = Vector2::ZERO, const Vector4& color = Vector4::ONE);
	void ClearRenderSet();
	void Render(bool isRenderClear = true);

	bool Add(const wchar_t* str);
	bool Remove(const wchar_t* str);
	bool Find(const wchar_t* str);
	float GetWidth(const WCHAR* str);
	float GetWidth(const UINT index) const;
	int GetFontSize() const { return mFontSize; }
	size_t GetNum() const { return mFonts.size(); }
};