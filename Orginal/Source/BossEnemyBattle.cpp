#include "BossEnemyBattle.h"

#include "CommandBossEnemy.h"
#include "Define.h"
#include "Enemy.h"
#include "Singleton.h"
#include "StatusData.h"

BossEnemyBattle::BossEnemyBattle(const Enemy* enemy) : BattleCharacter(enemy, Singleton<DataBase>().GetInstance().GetStatusData()->GetEnmStatus(enemy->GetCharaID()))
{
	mCommand = std::make_unique<CommandBossEnemy>();
}

BossEnemyBattle::~BossEnemyBattle()
{
}

void BossEnemyBattle::Initialize()
{
	SetMotion(SkinnedMesh::IDLE);
	SetAngle(Vector3(0.0f, Define::PI, 0.0f));
}