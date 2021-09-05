#pragma once
#include "Enemy.h"

class BossEnemy : public Enemy
{
	static constexpr float MASS = 1000.0f;

public:
	BossEnemy(int charaID);
	~BossEnemy();

	void Initialize() override;
	void Update(const Vector3& playerPos) override;
};