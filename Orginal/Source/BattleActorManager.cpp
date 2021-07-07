#include "BattleActorManager.h"

#include <map>
#include <random>

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleState.h"
#include "Enemy.h"
#include "EnemyBattle.h"
#include "Fade.h"
#include "PlayerBattle.h"
#include "PlayerManager.h"
#include "SceneManager.h"

BattleActorManager::BattleActorManager(PlayerManager* player, Enemy* enemy)
{
	mHitEnemy = enemy;

	// �v���C���[�o�^
	mPlayerNum = player->GetNum();
	for (size_t i = 0; i < mPlayerNum; ++i)
	{
		PlayerCreateAndRegister(player->GetPlayer(i));
	}

	// �G�o�^
	EnemyCreateAndRegister(enemy);
}

void BattleActorManager::Initialize()
{
	mCharacterHealth.Initialize(Vector2(HEALTH_PLATE_X, HEALTH_PLATE_Y));
	mTurnManager.Initialize(mBActors);

	for (auto& ba : mBActors) ba->Initialize();
	// ���W�ݒ�
	{
		// PLAYER
		size_t size = mAliveActorIDs[Actor::Type::PLAYER].size();
		for (size_t i = 0; i < size; ++i)
		{
			// ����1�l�Ȃ̂ŉ�
			Vector3 pos(0, 0, PLAYER_POS_Z);
			mBActors[mAliveActorIDs[Actor::Type::PLAYER][i]]->SetPos(pos);
			mBActors[mAliveActorIDs[Actor::Type::PLAYER][i]]->UpdateWorld();
		}

		// ENEMY
		size = mAliveActorIDs[Actor::Type::ENEMY].size();
		for (size_t i = 0; i < size; ++i)
		{
		// ����1�l�Ȃ̂ŉ�
		Vector3 pos(0, 0, ENEMY_POS_Z);
		mBActors[mAliveActorIDs[Actor::Type::ENEMY][i]]->SetPos(pos);
		mBActors[mAliveActorIDs[Actor::Type::ENEMY][i]]->UpdateWorld();
		}

	}
}

void BattleActorManager::Update()
{
	if (mTurnManager.IsResult())
	{
		// ���[�V�����̍X�V��������
		for (auto& actor : mBActors) actor->UpdateWorld();

		// field�ɖ߂�
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			if (Fade::GetInstance().SetSceneImage(Fade::SPEED_SLOW))
			{
				SceneManager::GetInstance().PopCurrentScene();
			}
		}
	}
	else
	{
		BattleActor* moveActor = mTurnManager.GetMoveActor();
		moveActor->Update(this);

		// MoveActor�ȊO�����[���h, ���[�V�����̍X�V������
		for (auto& actor : mBActors)
		{
			if (moveActor == actor.get()) continue;
			actor->UpdateWorld();
		}

		// healthplate�X�V
		{
			std::vector<Status> statusArray;
			for (int i = 0; i < mPlayerNum; ++i)
			{
				// BActors�͍ŏ��Ƀv���C���[���͂����Ă邩�� 0~�l�����őS���ɃA�N�Z�X�ł���
				statusArray.push_back(*mBActors[i]->GetStatus());
			}
			mCharacterHealth.Update(statusArray);
		}


		mTurnManager.Update(this);

		// 1�^�[�����I��������
		if (mTurnManager.IsTurnFinished())
		{
			// ����ł�A�N�^�[���`�F�b�N
			OrganizeActor();

			// �o�g���I�����`�F�b�N
			Result result = CheckBattleFinish();
			if (result == PLAYER_WIN || result == PLAYER_LOSE) // TODO : ���󕉂������̓��ʂȏ����͂Ȃ�(�����Ă������Ă��ꏏ)
			{
				BattleState::GetInstance().SetState(BattleState::State::RESULT);
				mHitEnemy->SetExist(false);
				
				// ���U���g�Ɉڍs
				mTurnManager.ToResult();

				// �퓬��̃X�e�[�^�X���X�V
				for (int i = 0; i < mPlayerNum; ++i)
				{
					BattleActor* pl = mBActors[i].get();
					Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(pl->GetCharaID(), *pl->GetStatus());
				}

				// BGM�����U���g�̂�ɂ���
				AUDIO.MusicStop((int)Music::BATTLE);
				AUDIO.MusicPlay((int)Music::RESULT);
			}
		}
	}


}

void BattleActorManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBActors) ba->Render(view, projection, lightDir);

	// ���U���g����Ȃ��Ȃ�UIĶ��\��
	if (!mTurnManager.IsResult())
	{
		mTurnManager.GetMoveActor()->RenderCommand();    // MoveActor�̃R�}���hUI��\��
		mCharacterHealth.Render(false); // �L������HP��\��
		mTurnManager.Render();
	}
}

void BattleActorManager::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBActors) ba->Render(shader, view, projection, lightDir);
}

void BattleActorManager::PlayerCreateAndRegister(Player* pl)
{
	int objID = mBActors.size();
	mBActors.emplace_back(std::make_shared<PlayerBattle>(pl));
	mBActors.back()->SetObjID(objID);
	mAliveActorIDs[mBActors.back()->GetType()].push_back(objID);
}

void BattleActorManager::EnemyCreateAndRegister(Enemy* enm)
{
	int objID = mBActors.size();
	mBActors.emplace_back(std::make_shared<EnemyBattle>(enm));
	mBActors.back()->SetObjID(objID);
	mAliveActorIDs[mBActors.back()->GetType()].push_back(objID);
}

BattleActorManager::Result BattleActorManager::CheckBattleFinish()
{
	// mAliveActorIDs���`�F�b�N����
	Result ret = NONE;
	if (mAliveActorIDs[Actor::ENEMY].empty()) ret = PLAYER_WIN;
	if (mAliveActorIDs[Actor::PLAYER].empty()) ret = PLAYER_LOSE;

	return ret;
}

void BattleActorManager::OrganizeActor()
{
	// �̗͂�0�ɂȂ����A�N�^�\�𐮗�����
	for (auto& ba : mBActors)
	{
		// �̗͂�1�ȏ�Ȃ�continue
		if (!ba->GetStatus()->IsDead()) continue;

		// 0�ȉ��Ȃ� mAliveActorIDs �������
		auto& ids = mAliveActorIDs[ba->GetType()];
		for (auto it = ids.begin(); it != ids.end(); ++it)
		{
			if (*it == ba->GetObjID())
			{
				ids.erase(it);
				break;
			}
		}
	}
}

//void BattleActorManager::DecideMoveActor()
//{
//	if (mOrder.empty())
//	{
//		SortOrder();
//	}
//
//	while (true)
//	{
//		// ���Ԃ������L�������|����Ă���Ȃ疳��
//		if (mOrder.front()->GetObjID() == -1)
//		{
//			mOrder.pop();
//			if (mOrder.empty())
//			{
//				SortOrder();
//				break;
//			}
//		}
//		else break;
//	}
//
//	mMoveActor = mOrder.front();
//	mOrder.pop();
//}
