#pragma once
#include "Enemy.h"

class BossEnemy : public Enemy
{
public:
	BossEnemy(int charaID);
	~BossEnemy();

	void Initialize() override;
	void Update(const Vector3& playerPos) override;
};