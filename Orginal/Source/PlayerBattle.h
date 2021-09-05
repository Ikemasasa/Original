#pragma once
#include <memory>

#include "BattleCharacter.h"

class Player;

class PlayerBattle : public BattleCharacter
{
	static const Vector3 PLAYER_POS;

public:
	PlayerBattle(Player* player);
	~PlayerBattle() = default;

	void Initialize() override;
};