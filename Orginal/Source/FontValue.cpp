#include "FontValue.h"

#include <string>

#include "CameraManager.h"
#include "Singleton.h"

FontValue::FontValue()
{
	mFont.Initialize(FONT_SIZE, FONT_WEIGHT);
	
	const int STR_NUM = 10;
	const wchar_t* str[STR_NUM] = {
		L"0", L"1", L"2", L"3" ,L"4", L"5", L"6", L"7", L"8", L"9"
	};

	for (int i = 0; i < STR_NUM; ++i)
	{
		mFont.Add(str[i]);
	}
}

FontValue::~FontValue()
{
	mFont.Release();
}

void FontValue::RenderSet(const int value, const Vector3& pos, const Vector2& scale, const Vector2& centerPercent, const Vector4& color)
{
	std::wstring valueStr = std::to_wstring(value);
	const int digitNum = valueStr.size(); // åÖêî
	const float width = mFont.GetWidth(valueStr.c_str());
	const float widthPerWord = width / digitNum;

	// pos
	Vector2 scrPos;
	{
		scrPos = pos.WorldToScreen(Singleton<CameraManager>().GetInstance().GetView(), Singleton<CameraManager>().GetInstance().GetProj());
	}

	// center
	Vector2 center;
	{
		center.x = widthPerWord * centerPercent.x; // ç°ÇÕYñ¢é¿ëïÇ»ÇÃÇ≈XÇæÇØ
		center.y = 0.0f;
	}

	for (int i = 0; i < digitNum; ++i)
	{
		std::wstring val;
		val = valueStr[i];
		mFont.RenderSet(val.c_str(), Vector2(scrPos.x + widthPerWord * i, scrPos.y), center, scale, color);
	}
}

void FontValue::RenderSet(const int value, const Vector2& pos, const Vector2& scale, const Vector2& centerPercent, const Vector4& color)
{
	std::wstring valueStr = std::to_wstring(value);
	const size_t digitNum = valueStr.size(); // åÖêî
	const float width = mFont.GetWidth(valueStr.c_str());
	const float widthPerWord = width / digitNum;

	// center
	Vector2 center;
	{
		center.x = widthPerWord * centerPercent.x; // ç°ÇÕYñ¢é¿ëïÇ»ÇÃÇ≈XÇæÇØ
		center.y = 0.0f;
	}

	for (int i = 0; i < digitNum; ++i)
	{
		std::wstring val;
		val = valueStr[i];
		mFont.RenderSet(val.c_str(), Vector2(pos.x + widthPerWord * i, pos.y), center, scale, color);
	}
}

void FontValue::Render()
{
	mFont.Render();
}
