#include "CharacterManager.h"

#include "lib/Audio.h"

#include "Collision.h"
#include "CollisionTerrain.h"
#include "EnemyManager.h"
#include "Fade.h"
#include "Player.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "SceneBattle.h"
#include "Terrain.h"

CharacterManager::CharacterManager()
{
	mPlayerManager = std::make_unique<PlayerManager>();
	mEnemyManager = std::make_unique<EnemyManager>();
}

void CharacterManager::Initialize()
{
	mPlayerManager->Initialize();
	mEnemyManager->Initialize();
}

void CharacterManager::Update()
{
	mPlayerManager->Update();

	mEnemyManager->SetPlayerPos(mPlayerManager->GetMovePlayer()->GetPos());
	mEnemyManager->Update();


	// 追々8分木とかしてみたい
	Player* player = mPlayerManager->GetMovePlayer();
	for (int i = 0; i < mEnemyManager->GetNum(); ++i)
	{
		Enemy* enemy = mEnemyManager->GetEnemy(i);
		if (Collision::ColCapsules(enemy->GetCapsule(), player->GetCapsule()))
		{
			// 無敵時間じゃなければ
			if (!mPlayerManager->IsInvincible()) 
			{
				// 戻ってきた時用に無敵をonにする
				mPlayerManager->EnableInvincible();

				AUDIO.MusicStop((int)Music::FIELD_REMAINS);
				Fade::GetInstance().SetSceneImage(Fade::SPEED_SLOW);
				SceneManager::GetInstance().SetStackScene(std::make_unique<SceneBattle>(mPlayerManager.get(), mEnemyManager->GetEnemy(i)));
				break;
			}
			else
			{
				// 無敵時間中なら押し合いの計算をする
				Collide(player, enemy);
			}
		}
	}
}

void CharacterManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	mPlayerManager->Render(view, projection, lightDir);
	mEnemyManager->Render(view, projection, lightDir);
}

void CharacterManager::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	mPlayerManager->Render(shader, view, projection, lightDir);
	mEnemyManager->Render(shader, view, projection, lightDir);
}

void CharacterManager::Collide(Character* a, Character* b)
{
	float massA = a->GetMass();
	float massB = b->GetMass();
	float totalMass = massA + massB;

	float rateA = 0.0f, rateB = 0.0f;
	if (totalMass <= 0.0f)
	{
		rateA = rateB = 0.5f;
	}
	else
	{
		rateA = massA / totalMass;
		rateB = massB / totalMass;
	}

	// rateに応じてそれぞれ移動
	float totalSize = a->GetCapsule().radius + b->GetCapsule().radius;
	Vector3 v = b->GetPos() - a->GetPos();
	v.y = 0.0f; // 高さ無視
	float dist = v.Length();

	a->SetPos(a->GetPos() - v * (totalSize - dist) * rateA);
	b->SetPos(b->GetPos() + v * (totalSize - dist) * rateB);

}