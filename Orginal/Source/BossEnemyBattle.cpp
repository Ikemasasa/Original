#include "BossEnemyBattle.h"

#include "CommandBossEnemy.h"
#include "Define.h"
#include "Enemy.h"
#include "StatusData.h"

BossEnemyBattle::BossEnemyBattle(const Enemy* enemy) : BattleCharacter(enemy, StatusData::GetEnmStatus(enemy->GetCharaID()))
{
	// �R�}���h����N���X���쐬
	mCommand = std::make_unique<CommandBossEnemy>();
}

BossEnemyBattle::~BossEnemyBattle()
{
}

void BossEnemyBattle::Initialize()
{
	// �e�평����
	SetMotion(Character::IDLE);
	SetAngle(Vector3(0.0f, Define::PI, 0.0f));
}