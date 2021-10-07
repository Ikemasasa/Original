#pragma once

#include "Character.h"
#include "Item.h"

class Player : public Character
{
	static constexpr float MOVE_SPEED = 0.28f;
	static constexpr float MASS = 100.0f;
	static constexpr float RAYPICK_DIST = 0.5f;
	static constexpr float POS_Y_ADJUST_FACTOR = 0.25f;
	Item mInventory;

public:
	Player(int charaID);
	~Player() = default;

	void Initialize();
	void Update();

	Item* GetInventory() { return &mInventory; }
	void SetInventory(const Item* inventory) { mInventory = *inventory; }
};