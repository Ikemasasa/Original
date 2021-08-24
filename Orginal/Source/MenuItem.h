#pragma once
#include "MenuBase.h"

#include <memory>
#include <vector>

#include "lib/Font.h"

#include "MenuCharacterSelect.h"
#include "CharacterHealth.h"
#include "ItemSelect.h"

class Item;

class MenuItem : public MenuBase
{
	static constexpr float BOARD_OFFSET_X = 300.0f;
	static constexpr float BOARD_OFFSET_Y = 150.0f;

	static constexpr float HEALTH_BOARD_X = 900.0f;
	static constexpr float HEALTH_BOARD_Y = 100.0f;

private:
	Item* mInventory;

	int mItemIndex;
	MenuCharacterSelect mCharacterSelect;
	CharacterHealth mCharacterHealth;
	ItemSelect mItemSelect;

public:
	MenuItem() = default;
	~MenuItem() = default;

	void Initialize(const PlayerManager* plm) override;
	Select Update(PlayerManager* plm) override;
	void Render() override;
	void Release() override;
};