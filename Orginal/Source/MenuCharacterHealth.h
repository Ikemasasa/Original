#pragma once
#include <memory>
#include <string>

#include "lib/Font.h"

#include "FontValue.h"

class PlayerManager;
class Texture;

class MenuCharacterHealth
{
	static const int VERTICAL_NUM = 3;

	struct HealthPlate
	{
		// Ç±ÇÃï”ÇÃêîéöÇÕìKìñ
		static constexpr float FIRST_OFFSET_X = 16.0f;
		static constexpr float CUR_OFFSET_X = 100.0f;
		static constexpr float DELIM_OFFSET_X = 4.0f;
		static constexpr float MAX_OFFSET_X = 64.0f;

		static constexpr float FIRST_OFFSET_Y = 12.0f;
		static constexpr float ADD_OFFSET_Y = 32.0f;

		std::wstring name;
	};

	std::unique_ptr<Texture> mHealthPlate;
	std::unique_ptr<Texture> mSelect;

	Font mPlNameFont;
	Font mStatusNameFont;
	Font mDelimFont;
	FontValue mFontValue;
	std::vector<HealthPlate> mHealthPlates;
	Vector2 mPlateLeftTop;
	int mSelectIndex;
public:
	MenuCharacterHealth() = default;
	~MenuCharacterHealth() = default;

	void Initialize(const PlayerManager* plm, const Vector2& leftTop);
	void Update();
	void Render(bool isSelectRender = false);
	void Release();

	int GetSelectIndex() const { return mSelectIndex; }
};