#pragma once
#include "MenuBase.h"

#include <vector>

#include "CharacterHealth.h"
#include "ItemSelect.h"
#include "MenuCharacterSelect.h"
#include "SelectOptions.h"

class Item;
class PlayerManager;

class MenuItem : public MenuBase
{
	static constexpr float BOARD_OFFSET_X = 300.0f;
	static constexpr float BOARD_OFFSET_Y = 150.0f;

	static constexpr float OPTION_BOARD_X = 400.0f;
	static constexpr float OPTION_BOARD_Y = 250.0f;

	static constexpr float HEALTH_BOARD_X = 900.0f;
	static constexpr float HEALTH_BOARD_Y = 100.0f;

private:
	Item* mInventory;

	int mItemIndex;
	bool mIsUseFieldItem;
	MenuCharacterSelect mCharacterSelect;
	CharacterHealth mCharacterHealth;
	ItemSelect mItemSelect;
	SelectOptions mSelectOptions;

private:
	void UseHealItem(const PlayerManager* plm, const int itemID);
	void UseFieldUseItem(const int itemID);

public:
	MenuItem() = default;
	~MenuItem() = default;

	void Initialize(const PlayerManager* plm) override;
	Select Update(PlayerManager* plm) override;
	void Render() override;
	void Release() override;
};