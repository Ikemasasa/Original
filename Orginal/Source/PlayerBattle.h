#pragma once
#include <memory>

#include "BattleCharacter.h"

class Player;


// �v���C���[�̐퓬�p�L�����N�^�[�N���X
class PlayerBattle : public BattleCharacter
{

public:
	PlayerBattle(Player* player);
	~PlayerBattle() = default;

	void Initialize() override;
};