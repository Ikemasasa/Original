#pragma once
#include "MenuBase.h"

#include "lib/Font.h"

#include "FontValue.h"
#include "MenuCharacterSelect.h"

class MenuStatus : public MenuBase
{
	static constexpr float PLATE_X = 512.0f;
	static constexpr float PLATE_Y = 128.0f;

	static constexpr float NAME_OFFSET_Y = 10.0f;
	static constexpr float STATUS_OFFSET_X = 20.0f;
	static constexpr float CUR_OFFSET_X = 96.0f;
	static constexpr float DELIM_OFFSET_X = 4.0f;
	static constexpr float MAX_OFFSET_X = 12.0f;
	
	static constexpr float STATUS_OFFSET_Y = 60.0f;
	static constexpr float STATUS_ADD_Y = 34.0f;

	enum StatusKind
	{
		NAME,
		HP,
		MP,
		ATK,
		DEF,
		SPD,
		STATUS_NUM
	};

	Font mFont;
	FontValue mFontValue;

	MenuCharacterSelect mCharacterSelect;
	std::unique_ptr<Sprite> mStatusPlate;

public:
	MenuStatus() = default;
	~MenuStatus() = default;

	void Initialize(const PlayerManager* plm) override;
	Select Update(PlayerManager* plm) override;
	void Render() override;
	void Release() override;
};