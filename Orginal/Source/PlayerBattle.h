#pragma once
#include <memory>

#include "BattleActor.h"

class Player;
class Item;

class PlayerBattle : public BattleActor
{
	static const Vector3 PLAYER_POS;

public:
	PlayerBattle(Player* player);
	~PlayerBattle() = default;

	void Initialize() override;


	Item* GetInventory() const { return mInventory; }
};