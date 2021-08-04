#pragma once
#include <memory>

#include "BattleCharacter.h"

class Enemy;

class EnemyBattle : public BattleCharacter
{
	static const Vector3 ENEMY_POS;

public:
	EnemyBattle(const Enemy* enemy);
	~EnemyBattle() = default;

	void Initialize() override;
};