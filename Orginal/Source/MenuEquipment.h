#pragma once
#include "MenuBase.h"

#include "lib/Font.h"

#include "EquipmentSelect.h"
#include "MenuCharacterSelect.h"

class Sprite;

class MenuEquipment : public MenuBase
{
	static constexpr float FONT_OFFSET_X = 10.0f;
	static constexpr float FONT_OFFSET_Y = 10.0f;
	static constexpr float FONT_ADD_Y    = 32.0f;

	static constexpr float BOARD_POS_X = 150.0f;
	static constexpr float BOARD_POS_Y = 150.0f;

	MenuCharacterSelect mCharacterSelect;
	EquipmentSelect mEquipmentSelect;

	std::unique_ptr<Sprite> mTypeBoard;
	std::unique_ptr<Sprite> mTypeSelect;

	Font mNameFont;
	Font mFont;
	bool mIsDecideType;
	int mOldSelectIndex;
public:
	void Initialize(const PlayerManager* plm) override;
	Select Update(PlayerManager* plm) override;
	void Render() override;
	void Release() override;
};