#pragma once
#include <memory>

#include "BattleCharacter.h"

class Player;


// プレイヤーの戦闘用キャラクタークラス
class PlayerBattle : public BattleCharacter
{

public:
	PlayerBattle(Player* player);
	~PlayerBattle() = default;

	void Initialize() override;
};