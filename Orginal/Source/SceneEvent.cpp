#include "SceneEvent.h"

#include "Enemy.h"
#include "EnemyManager.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Terrain.h"


SceneEvent::SceneEvent(int eventID, PlayerManager* plm, EnemyManager* em, Terrain* terrain)
{
	mPlayerManager = std::make_unique<PlayerManager>();
	mEnemyManager = std::make_unique<EnemyManager>();
	mTerrain = std::make_unique<Terrain>(terrain->GetID());

	for (size_t i = 0; i < plm->GetNum(); ++i)
	{
		Player* fieldPl = plm->GetPlayer(i);

		mPlayerManager->Create(fieldPl->GetCharaID());
		Player* pl = mPlayerManager->GetPlayer(i);
		pl->SetPos(fieldPl->GetPos());
		pl->SetAngle(pl->GetAngle());
	}

	for (size_t i = 0; i < em->GetNum(); ++i)
	{
		mEnemyManager->Create(em->GetEnemy(i)->GetCharaID());
	}
}

SceneEvent::~SceneEvent()
{
}

void SceneEvent::Initialize()
{
}

void SceneEvent::Update()
{
}

void SceneEvent::Render()
{
}

void SceneEvent::Release()
{
}
