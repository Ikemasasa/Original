#include "FontValue.h"

#include <string>

#include "CameraManager.h"
#include "Singleton.h"

FontValue::FontValue()
{

}

FontValue::~FontValue()
{
	mFont.Release();
}

void FontValue::Initialize(int fontSize, int fontWeight)
{
	mFont.Initialize(fontSize, fontWeight);

	const int STR_NUM = 10;
	const wchar_t* str[STR_NUM] = {
		L"0", L"1", L"2", L"3" ,L"4", L"5", L"6", L"7", L"8", L"9"
	};

	for (int i = 0; i < STR_NUM; ++i)
	{
		mFont.Add(str[i]);
	}
}

void FontValue::RenderSet(const int value, const Vector3& pos, const Vector2& center, const Vector2& scale, const Vector4& color)
{
	std::wstring valueStr = std::to_wstring(value);
	const int digitNum = valueStr.size(); // Œ…”
	const float width = mFont.GetWidth(valueStr.c_str());
	const float widthPerWord = width / digitNum;

	// pos
	Vector2 scrPos;
	{
		scrPos = pos.WorldToScreen(Singleton<CameraManager>().GetInstance().GetView(), Singleton<CameraManager>().GetInstance().GetProj());
	}

	for (int i = 0; i < digitNum; ++i)
	{
		std::wstring val;
		val = valueStr[i];
		mFont.RenderSet(val.c_str(), Vector2(scrPos.x + widthPerWord * i, scrPos.y), center, scale, color);
	}
}

void FontValue::RenderSet(const int value, const Vector2& pos, const Vector2& center, const Vector2& scale, const Vector4& color)
{
	std::wstring valueStr = std::to_wstring(value);
	const size_t digitNum = valueStr.size(); // Œ…”
	const float width = mFont.GetWidth(valueStr.c_str());
	const float widthPerWord = width / digitNum;

	for (int i = 0; i < digitNum; ++i)
	{
		std::wstring val;
		val = valueStr[i];
		mFont.RenderSet(val.c_str(), Vector2(pos.x + widthPerWord * i, pos.y), center, scale, color);
	}
}

void FontValue::Render(bool isRenderClear)
{
	mFont.Render(isRenderClear);
}

void FontValue::Release()
{
	mFont.Release();
}