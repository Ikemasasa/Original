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

	// �����蔻��
	CollideNPC();
	CollideEnemy();

	// ��b���X�V
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
	// �����蔻��
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
			// ���G���Ԃ���Ȃ����
			if (!mPlayerManager->IsInvincible())
			{
				// �߂��Ă������p�ɖ��G��on�ɂ���
				mPlayerManager->EnableInvincible();

				Fade::GetInstance().SetSceneImage(Fade::SPEED_SLOW);
				SceneManager::GetInstance().SetStackScene(std::make_unique<SceneBattle>(mPlayerManager.get(), mEnemyManager->GetEnemy(i)));
				break;
			}
			else
			{
				// ���G���Ԓ��Ȃ牟�������̌v�Z������
				if (enemy->GetEnmType() == StatusData::EnemyType::BOSS) CollideObject(player, enemy);
				else 													Collide(player, enemy);
			}
		}
	}
}

void CharacterManager::TalkCheck()
{
	// ���łɉ�b���Ȃ�return
	if (mNPCTalk->IsTalking()) return;

	// NPC�Ƃ̉�b�̃`�F�b�N
	Player* player = mPlayerManager->GetMovePlayer();
	Vector3 plFront(sinf(player->GetAngle().y), 0.0f, cosf(player->GetAngle().y));
	for (size_t i = 0; i < mNPCManager->GetNum(); ++i)
	{
		NPC* npc = mNPCManager->GetNPC(i);

		// �����`�F�b�N
		Vector3 dist = npc->GetPos() - player->GetPos();
		if (dist.LengthSq() <= pow(NPCManager::TALK_DIST, 2))
		{
			// ��b���邩�`�F�b�N
			if (Input::GetButtonTrigger(0, Input::BUTTON::A))
			{
				// �����`�F�b�N
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

	// rate�ɉ����Ă��ꂼ��ړ�
	float totalSize = a->GetCapsule().radius + b->GetCapsule().radius;
	Vector3 v = b->GetPos() - a->GetPos();
	v.y = 0.0f; // ��������
	float dist = v.Length();

	a->SetPos(a->GetPos() - v * (totalSize - dist) * rateA);
	b->SetPos(b->GetPos() + v * (totalSize - dist) * rateB);

}

void CharacterManager::CollideObject(Character* move, Character* object)
{
	// mass �֌W�Ȃ��ɓ������Ɠ����Ȃ���

	float totalSize = move->GetCapsule().radius + object->GetCapsule().radius;
	Vector3 v = move->GetPos() - object->GetPos();
	v.y = 0.0f; // ��������
	float dist = v.Length();

	move->SetPos(move->GetPos() + v * (totalSize - dist));
}
