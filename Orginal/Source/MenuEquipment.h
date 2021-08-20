#pragma once
#include "MenuBase.h"

#include "lib/Font.h"

#include "EquipmentSelect.h"
#include "MenuCharacterSelect.h"

class Sprite;

class MenuEquipment : public MenuBase
{
	static constexpr float FIRST_OFFSET_X = 16.0f;
	static constexpr float FIRST_OFFSET_Y = 12.0f;
	static constexpr float ADD_OFFSET_Y = 33.0f;

	static constexpr float BOARD_POS_X = 150.0f;
	static constexpr float BOARD_POS_Y = 150.0f;

	MenuCharacterSelect mCharacterSelect;
	EquipmentSelect mEquipmentSelect;

	std::unique_ptr<Sprite> mBoard;
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