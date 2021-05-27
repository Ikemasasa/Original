#pragma once
#include <memory>

#include "BattleActor.h"

class EnemyBattle : public BattleActor
{
	static const Vector3 ENEMY_POS;

public:
	EnemyBattle(const std::shared_ptr<Actor>& enemy);
	~EnemyBattle() = default;

	void Initialize() override;
};