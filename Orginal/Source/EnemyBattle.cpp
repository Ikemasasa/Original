#include "EnemyBattle.h"

#include <memory>

#include "CommandEnemy.h"
#include "Define.h"
#include "Enemy.h"
#include "DataBase.h"
#include "Singleton.h"
#include "StatusData.h"

EnemyBattle::EnemyBattle(const Enemy* enemy) : BattleCharacter(enemy, Singleton<DataBase>().GetInstance().GetStatusData()->GetEnmStatus(enemy->GetCharaID()))
{
	mCommand = std::make_unique<CommandEnemy>();// 後でコマンドエネミーにする
}

void EnemyBattle::Initialize()
{
	SetMotion(SkinnedMesh::IDLE);
	SetAngle(Vector3(0.0f, Define::PI, 0.0f));
}