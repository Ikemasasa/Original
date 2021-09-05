#pragma once
#include "BattleCharacter.h"

class Enemy;

class EnemyBattle : public BattleCharacter
{

public:
	EnemyBattle(const Enemy* enemy);
	~EnemyBattle() = default;

	void Initialize() override;
};