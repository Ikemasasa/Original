#pragma once
#include <memory>

#include "Character.h"
#include "Item.h"
#include "StatusData.h"

#include "lib/Vector.h"
#include "lib/GeometricPrimitive.h"

class CameraBase;

class Player : public Character
{
	static constexpr float MOVE_SPEED = 0.3f;

	Item mInventory;

public:
	Player(int charaID);
	~Player() = default;

	void Initialize();
	void Update();

	Item* GetInventory() { return &mInventory; }
};