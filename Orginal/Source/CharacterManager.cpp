#include "CharacterManager.h"

#include "lib/Audio.h"
#include "lib/Input.h"

#include "Collision.h"
#include "CollisionTerrain.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Fade.h"
#include "NPC.h"
#include "NPCManager.h"
#include "Player.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "SceneBattle.h"
#include "Terrain.h"

CharacterManager::CharacterManager()
{
	mPlayerManager = std::make_unique<PlayerManager>();
	mEnemyManager = std::make_unique<EnemyManager>();
	mNPCManager = std::make_unique<NPCManager>();
	mNPCTalk = std::make_unique<NPCTalk>();
}

void CharacterManager::Initialize()
{
	mPlayerManager->Initialize();
	mEnemyManager->Initialize();
	mNPCManager->Initialize();
	mNPCTalk->Initialize();
}

void CharacterManager::Update()
{
	mPlayerManager->Update(mNPCTalk->IsTalking());

	mEnemyManager->SetPlayerPos(mPlayerManager->GetMovePlayer()->GetPos());
	mEnemyManager->Update(mNPCTalk->IsTalking());
	
	mNPCManager->SetPlayerPos(mPlayerManager->GetMovePlayer()->GetPos());
	mNPCManager->Update();

	// 当たり判定
	CollideNPC();
	CollideEnemy();

	// 会話情報更新
	TalkCheck();
	mNPCTalk->Update();
}

void CharacterManager::Render(const Matrix& view, const Matrix& proj, const Vector4& lightDir)
{
	mPlayerManager->Render(view, proj, lightDir);
	mEnemyManager->Render(view, proj, lightDir);
	mNPCManager->Render(view, proj, lightDir);
}

void CharacterManager::Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir)
{
	mPlayerManager->Render(shader, view, proj, lightDir);
	mEnemyManager->Render(shader, view, proj, lightDir);
	mNPCManager->Render(shader, view, proj, lightDir);
}

void CharacterManager::RenderUI()
{
	mNPCManager->RenderUI();
	mNPCTalk->Render();
}

void CharacterManager::CollideNPC()
{
	// 当たり判定
	Player* player = mPlayerManager->GetMovePlayer();
	CAPSULE plCapsule = player->GetCapsule();
	Vector3 plFront(sinf(player->GetAngle().y), 0.0f, cosf(player->GetAngle().y));
	for (size_t i = 0; i < mNPCManager->GetNum(); ++i)
	{
		NPC* npc = mNPCManager->GetNPC(i);
		if (Collision::ColCapsules(npc->GetCapsule(), plCapsule))
		{
			CollideObject(player, npc);
		}
	}
}

void CharacterManager::CollideEnemy()
{
	Player* player = mPlayerManager->GetMovePlayer();
	CAPSULE plCapsule = player->GetCapsule();
	for (int i = 0; i < mEnemyManager->GetNum(); ++i)
	{
		Enemy* enemy = mEnemyManager->GetEnemy(i);
		if (Collision::ColCapsules(enemy->GetCapsule(), plCapsule))
		{
			// 無敵時間じゃなければ
			if (!mPlayerManager->IsInvincible())
			{
				// 戻ってきた時用に無敵をonにする
				mPlayerManager->EnableInvincible();

				Fade::GetInstance().SetSceneImage(Fade::SPEED_SLOW);
				SceneManager::GetInstance().SetStackScene(std::make_unique<SceneBattle>(mPlayerManager.get(), mEnemyManager->GetEnemy(i)));
				break;
			}
			else
			{
				// 無敵時間中なら押し合いの計算をする
				if (enemy->GetEnmType() == StatusData::EnemyType::BOSS) CollideObject(player, enemy);
				else 													Collide(player, enemy);
			}
		}
	}
}

void CharacterManager::TalkCheck()
{
	// すでに会話中ならreturn
	if (mNPCTalk->IsTalking()) return;

	// NPCとの会話のチェック
	Player* player = mPlayerManager->GetMovePlayer();
	Vector3 plFront(sinf(player->GetAngle().y), 0.0f, cosf(player->GetAngle().y));
	for (size_t i = 0; i < mNPCManager->GetNum(); ++i)
	{
		NPC* npc = mNPCManager->GetNPC(i);

		// 距離チェック
		Vector3 dist = npc->GetPos() - player->GetPos();
		if (dist.LengthSq() <= pow(NPCManager::TALK_DIST, 2))
		{
			// 会話するかチェック
			if (Input::GetButtonTrigger(0, Input::BUTTON::A))
			{
				// 向きチェック
				float dot = Vector3::Dot(plFront, dist.GetNormalize());
				if (dot >= NPCManager::TALK_DOT)
				{
					mNPCTalk->Set(npc, player->GetPos());
				}
			}
		}
	}
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
		rateA = massB / totalMass;
		rateB = massA / totalMass;
	}

	// rateに応じてそれぞれ移動
	float totalSize = a->GetCapsule().radius + b->GetCapsule().radius;
	Vector3 v = b->GetPos() - a->GetPos();
	v.y = 0.0f; // 高さ無視
	float dist = v.Length();

	a->SetPos(a->GetPos() - v * (totalSize - dist) * rateA);
	b->SetPos(b->GetPos() + v * (totalSize - dist) * rateB);

}

void CharacterManager::CollideObject(Character* move, Character* object)
{
	// mass 関係なしに動く方と動かない方

	float totalSize = move->GetCapsule().radius + object->GetCapsule().radius;
	Vector3 v = move->GetPos() - object->GetPos();
	v.y = 0.0f; // 高さ無視
	float dist = v.Length();

	move->SetPos(move->GetPos() + v * (totalSize - dist));
}
