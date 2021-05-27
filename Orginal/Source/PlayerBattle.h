#pragma once
#include <memory>

#include "BattleActor.h"
#include "CommandPlayer.h"
#include "SceneBattle.h"

class PlayerBattle : public BattleActor
{
	static const Vector3 PLAYER_POS;

public:
	PlayerBattle(const std::shared_ptr<Actor>& player);
	~PlayerBattle() = default;

	void Initialize() override;
};