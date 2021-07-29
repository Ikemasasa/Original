#include "ActorManager.h"

#include "lib/Audio.h"

#include "Collision.h"
#include "CollisionTerrain.h"
#include "EnemyManager.h"
#include "Fade.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "SceneBattle.h"
#include "Terrain.h"

ActorManager::ActorManager()
{
	mPlayerManager = std::make_unique<PlayerManager>();
	mEnemyManager = std::make_unique<EnemyManager>();

	mTerrain = std::make_unique<Terrain>(DataBase::TERRAIN_ID_START);
	CollisionTerrain::RegisterTerrain(mTerrain.get());
}

void ActorManager::Initialize()
{
	mTerrain->Initialize();
	mPlayerManager->Initialize();
	mEnemyManager->Initialize();
}

void ActorManager::Update()
{
	mPlayerManager->Update();

	mEnemyManager->SetPlayerPos(mPlayerManager->GetMovePlayer()->GetPos());
	mEnemyManager->Update();


	// ’ÇX8•ª–Ø‚Æ‚©‚µ‚Ä‚Ý‚½‚¢
	// –³“GŽžŠÔ’†‚È‚ç“–‚½‚ç‚È‚¢
	if (mPlayerManager->IsInvincible()) return;
	for (int i = 0; i < mEnemyManager->GetNum(); ++i)
	{
		if (Collision::ColCapsules(mEnemyManager->GetEnemy(i)->GetCapsule(), mPlayerManager->GetMovePlayer()->GetCapsule()))
		{
			// –ß‚Á‚Ä‚«‚½Žž—p‚É–³“G‚ðon‚É‚·‚é
			mPlayerManager->EnableInvincible();

			AUDIO.MusicStop((int)Music::FIELD_REMAINS);
			Fade::GetInstance().SetSceneImage(0.02f);
			SceneManager::GetInstance().SetStackScene(std::make_unique<SceneBattle>(mPlayerManager.get(), mEnemyManager->GetEnemy(i)));
			break;
		}
	}
}

void ActorManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	mTerrain->Render(view, projection, lightDir);
	mPlayerManager->Render(view, projection, lightDir);
	mEnemyManager->Render(view, projection, lightDir);
}

void ActorManager::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	mTerrain->Render(shader, view, projection, lightDir);
	mPlayerManager->Render(shader, view, projection, lightDir);
	mEnemyManager->Render(shader, view, projection, lightDir);
}

