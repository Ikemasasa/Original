#pragma once
#include "BattleCharacter.h"

class BattleCharacterManager;
class Enemy;

class BossEnemyBattle : public BattleCharacter
{

public:
	BossEnemyBattle(const Enemy* enemy);
	~BossEnemyBattle();

	void Initialize() override;

};