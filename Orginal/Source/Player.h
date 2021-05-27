#pragma once
#include <memory>

#include "Actor.h"

#include "Item.h"
#include "StatusData.h"

#include "lib/Vector.h"
#include "lib/GeometricPrimitive.h"

class CameraBase;

class Player : public Actor
{
	Status mStatus;
	Vector3 mVelocity;
	Item mInventory;

public:
	Player(int charaID);
	~Player() = default;

	void Initialize();
	void Update();

	Status* GetStatus() { return &mStatus; }
	Item* GetInventory() { return &mInventory; }
};