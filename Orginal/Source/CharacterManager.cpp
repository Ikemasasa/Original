#include "CharacterManager.h"

#include "lib/Audio.h"

#include "Collision.h"
#include "CollisionTerrain.h"
#include "EnemyManager.h"
#include "Fade.h"
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


	// �ǁX8���؂Ƃ����Ă݂���
	for (int i = 0; i < mEnemyManager->GetNum(); ++i)
	{
		if (Collision::ColCapsules(mEnemyManager->GetEnemy(i)->GetCapsule(), mPlayerManager->GetMovePlayer()->GetCapsule()))
		{
			// ���G���Ԃ���Ȃ����
			if (!mPlayerManager->IsInvincible()) 
			{
				// �߂��Ă������p�ɖ��G��on�ɂ���
				mPlayerManager->EnableInvincible();

				AUDIO.MusicStop((int)Music::FIELD_REMAINS);
				Fade::GetInstance().SetSceneImage(Fade::SPEED_FAST);
				SceneManager::GetInstance().SetStackScene(std::make_unique<SceneBattle>(mPlayerManager.get(), mEnemyManager->GetEnemy(i)));
				break;
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

