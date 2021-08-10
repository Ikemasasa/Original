#include "BattleCharacterManager.h"

#include <map>
#include <random>

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleState.h"
#include "DropData.h"
#include "EffectManager.h"
#include "Enemy.h"
#include "EnemyBattle.h"
#include "Fade.h"
#include "PlayerBattle.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "Singleton.h"

BattleCharacterManager::BattleCharacterManager(PlayerManager* pm, Enemy* enemy)
{
	mPlayerManager = pm;
	mHitEnemy = enemy;

	// �v���C���[�o�^
	mPlayerNum = pm->GetNum();
	for (int i = 0; i < mPlayerNum; ++i)
	{
		PlayerCreateAndRegister(pm->GetPlayer(i));
	}

	// �G�o�^
	EnemyCreateAndRegister(enemy);
}

void BattleCharacterManager::Initialize()
{
	mCharacterHealth.Initialize(Vector2(HEALTH_PLATE_X, HEALTH_PLATE_Y));
	mTurnManager.Initialize(mBCharacters);

	for (auto& ba : mBCharacters) ba->Initialize();
	// ���W�ݒ�
	{
		// TODO: ���f���̑傫�����l�����Ă��Ȃ�����A���f���ɂ������o�O��\������

		// PLAYER
		size_t size = mAliveCharaIDs[Character::Type::PLAYER].size();
		float offsetX = (POS_MAX_X - POS_MIN_X) / (size + 1);
		for (size_t i = 0; i < size; ++i)
		{
			Vector3 pos(POS_MIN_X + offsetX * (i + 1), 0, PLAYER_POS_Z);
			mBCharacters[mAliveCharaIDs[Character::Type::PLAYER][i]]->SetPos(pos);
			mBCharacters[mAliveCharaIDs[Character::Type::PLAYER][i]]->UpdateWorld();
		}

		// ENEMY
		size = mAliveCharaIDs[Character::Type::ENEMY].size();
		offsetX = (POS_MAX_X - POS_MIN_X) / (size + 1);
		for (size_t i = 0; i < size; ++i)
		{
			Vector3 pos(POS_MIN_X + offsetX * (i + 1), 0, ENEMY_POS_Z);
			mBCharacters[mAliveCharaIDs[Character::Type::ENEMY][i]]->SetPos(pos);
			mBCharacters[mAliveCharaIDs[Character::Type::ENEMY][i]]->UpdateWorld();
		}

	}

}

void BattleCharacterManager::Update()
{
	if (mTurnManager.IsResult())
	{
		// ���[�V�����̍X�V��������
		for (auto& chara : mBCharacters) chara->UpdateWorld();

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
		BattleCharacter* moveChara = mTurnManager.GetMoveChara();
		moveChara->Update(this);

		// MoveChara�ȊO�����[���h, ���[�V�����̍X�V������
		for (auto& chara : mBCharacters)
		{
			if (moveChara == chara.get()) continue;
			chara->UpdateWorld();
		}

		// healthplate�X�V
		{
			std::vector<Status> statusArray;
			for (int i = 0; i < mPlayerNum; ++i)
			{
				// BCharacters�͍ŏ��Ƀv���C���[���͂����Ă邩�� 0~�l�����őS���ɃA�N�Z�X�ł���
				statusArray.push_back(*mBCharacters[i]->GetStatus());
			}
			mCharacterHealth.Update(statusArray);
		}


		mTurnManager.Update(this);



		// 1�^�[�����I��������
		if (mTurnManager.IsTurnFinished())
		{
			// ����ł�A�N�^�[���`�F�b�N
			OrganizeCharacter();

			// �o�g���I�����`�F�b�N
			Result result = CheckBattleFinish();
			if (result == PLAYER_WIN) // TODO : ���󕉂������̓��ʂȏ����͂Ȃ�(�����Ă������Ă��ꏏ)
			{
				BattleState::GetInstance().SetState(BattleState::State::RESULT);
				mHitEnemy->SetExist(false);
				
				// ���U���g�Ɉڍs
				mTurnManager.ToResult();

				// �퓬��̃X�e�[�^�X���X�V
				for (int i = 0; i < mPlayerNum; ++i)
				{
					BattleCharacter* pl = mBCharacters[i].get();
					Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(pl->GetCharaID(), *pl->GetStatus());
				}

				// �h���b�v�A�C�e�����C���x���g���ɉ�����
				for (auto& dropID : mDropItemIDs)
				{
					mPlayerManager->GetEquipmentInventory()->Push(dropID);
				}

				// BGM�����U���g�̂�ɂ���
				AUDIO.MusicStop((int)Music::BATTLE);
				AUDIO.MusicPlay((int)Music::RESULT);
			}
			else
			{
				// �Ƃ肠�����^�C�g���ɖ߂�
			}
		}
	}


}

void BattleCharacterManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBCharacters) ba->Render(view, projection, lightDir);

	// ���U���g����Ȃ��Ȃ�UIĶ��\��
	if (!mTurnManager.IsResult())
	{
		mTurnManager.GetMoveChara()->RenderCommand();    // MoveChara�̃R�}���hUI��\��
		mCharacterHealth.Render(false); // �L������HP��\��
		mTurnManager.Render();
	}
}

void BattleCharacterManager::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBCharacters) ba->Render(shader, view, projection, lightDir);
}

void BattleCharacterManager::PlayerCreateAndRegister(Player* pl)
{
	int objID = mBCharacters.size();
	mBCharacters.emplace_back(std::make_shared<PlayerBattle>(pl));
	mBCharacters.back()->SetObjID(objID);
	mAliveCharaIDs[mBCharacters.back()->GetType()].push_back(objID);
}

void BattleCharacterManager::EnemyCreateAndRegister(Enemy* enm)
{
	int objID = mBCharacters.size();
	mBCharacters.emplace_back(std::make_shared<EnemyBattle>(enm));
	mBCharacters.back()->SetObjID(objID);
	mAliveCharaIDs[mBCharacters.back()->GetType()].push_back(objID);
}

BattleCharacterManager::Result BattleCharacterManager::CheckBattleFinish()
{
	// mAliveCharaIDs���`�F�b�N����
	Result ret = NONE;
	if (mAliveCharaIDs[Character::ENEMY].empty())  ret = PLAYER_WIN;
	if (mAliveCharaIDs[Character::PLAYER].empty()) ret = PLAYER_LOSE;

	return ret;
}

void BattleCharacterManager::OrganizeCharacter()
{
	// �̗͂�0�ɂȂ����A�N�^�\�𐮗�����
	for (auto& ba : mBCharacters)
	{
		// �̗͂�1�ȏ�Ȃ�continue, ���ł�exist��false�Ȃ�
		if (!ba->GetStatus()->IsDead()) continue;

		// �G�Ȃ�h���b�v�̃`�F�b�N������
		if (ba->GetType() == Character::ENEMY)
		{
			int dropItemID = 0;
			Singleton<DataBase>().GetInstance().GetDropData()->DecideDropItem(ba->GetCharaID(), &dropItemID);
			mDropItemIDs.push_back(dropItemID);
		}

		// 0�ȉ��Ȃ� mAliveCharaIDs �������
		auto& ids = mAliveCharaIDs[ba->GetType()];
		for (auto it = ids.begin(); it != ids.end(); ++it)
		{
			// �G�Ȃ�h���b�v�̃`�F�b�N������
			if (*it == ba->GetObjID())
			{
				ids.erase(it);
				break;
			}
		}
	}
}