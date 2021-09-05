#pragma once

#include "Character.h"
#include "Item.h"

class Player : public Character
{
	static constexpr float MOVE_SPEED = 0.3f;
	static constexpr float MASS = 100.0f;

	Item mInventory;

public:
	Player(int charaID);
	~Player() = default;

	void Initialize();
	void Update();

	Item* GetInventory() { return &mInventory; }
	void SetInventory(const Item* inventory) { mInventory = *inventory; }
};