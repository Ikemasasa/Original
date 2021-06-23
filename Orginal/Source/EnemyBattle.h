#pragma once
#include <memory>

#include "BattleActor.h"

class Enemy;

class EnemyBattle : public BattleActor
{
	static const Vector3 ENEMY_POS;

public:
	EnemyBattle(const Enemy* enemy);
	~EnemyBattle() = default;

	void Initialize() override;
};