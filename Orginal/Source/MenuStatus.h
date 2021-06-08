#pragma once
#include "MenuBase.h"

#include "lib/Font.h"

#include "FontValue.h"
#include "MenuCharacterSelect.h"

class MenuStatus : public MenuBase
{
	static constexpr float PLATE_X = 512.0f;
	static constexpr float PLATE_Y = 128.0f;

	static constexpr float FIRST_OFFSET_X = 16.0f;
	static constexpr float CUR_OFFSET_X = 128.0f;
	static constexpr float DELIM_OFFSET_X = 4.0f;
	static constexpr float MAX_OFFSET_X = 64.0f;

	static constexpr float FIRST_OFFSET_Y = 12.0f;
	static constexpr float ADD_OFFSET_Y = 33.0f;

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

	Font mPlNameFont;
	Font mStatusNameFont;
	Font mDelimFont;
	FontValue mFontValue;

	MenuCharacterSelect mCharacterSelect;

	std::unique_ptr<Texture> mStatusPlate;

public:
	MenuStatus() = default;
	~MenuStatus() = default;

	void Initialize(const PlayerManager* plm) override;
	Select Update(const PlayerManager* plm) override;
	void Render() override;
	void Release() override;
};