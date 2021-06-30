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

void BattleActorManager::SortOrder()
{
	auto RandArrayNoDuplicate = [](const int min, const int max)
	{
		const int dist = max - min + 1;

		std::vector<int> temp;
		temp.reserve(dist);
		for (int i = min; i <= max; ++i) temp.push_back(i);

		std::random_device seedGen;
		std::default_random_engine engine(seedGen());

		for (int i = dist - 1; i > 0; --i)
		{
			int target = std::uniform_int_distribution<int>(i, dist - 1)(engine);
			if (i != target) std::swap(temp[i], temp[target]);
		}

		return temp;
	};

	std::map<int, std::vector<BattleActor*>> agiOrder;

	// �}�b�v�̃L�[�͏����Ƀ\�[�g����Ă�
	for (auto& actor : mBActors)
	{
		agiOrder[actor->GetStatus()->agi].push_back(actor.get());
	}

	// �~���ɑ�����������烊�o�[�X�C�e���[�^
	for (auto it = agiOrder.rbegin(); it != agiOrder.rend(); ++it)
	{
		// 2�ȏ�Ȃ炻�̒����烉���_���Ō��߂�
		if (it->second.size() > 2)
		{
			std::vector<int> randArr = RandArrayNoDuplicate(0, it->second.size() - 1);
			for (size_t i = 0; i < it->second.size(); ++i)
			{
				mOrder.push(it->second.at(randArr[i]));
			}
		}
		else
		{
			mOrder.push(it->second.back());
		}
	}
}

BattleActorManager::BattleActorManager(PlayerManager* player, Enemy* enemy)
{
	mHitEnemy = enemy;

	// �v���C���[�o�^
	mPlayerNum = player->GetNum();
	for (size_t i = 0; i < mPlayerNum; ++i)
	{
		PlayerCreateAndRegister(player->GetPlayer(i));
	}
	mIsResult = false;

	// �G�o�^
	EnemyCreateAndRegister(enemy);

	SortOrder();
	mMoveActor = mOrder.front();
	mOrder.pop();
}

void BattleActorManager::Initialize()
{
	mCharacterHealth.Initialize(Vector2(HEALTH_PLATE_X, HEALTH_PLATE_Y));

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

	mProduction.Initialize();
}

void BattleActorManager::Update()
{
	if (mIsResult)
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
		// �A�N�^�[��update�����āAbehaviour�����܂�Ή��o�A ���̃A�N�^�[��
		bool isBehaviourEnable = false;
		isBehaviourEnable = mMoveActor->Update(this);

		// MoveActor�ȊO�����[�V�����̍X�V������
		for (auto& actor : mBActors)
		{
			if (mMoveActor == actor.get()) continue;
			actor->UpdateWorld();
		}

		// healthplate�X�V
		{
			Status* statusArray = new Status[mPlayerNum];
			for (int i = 0; i < mPlayerNum; ++i)
			{
				// BActors�͍ŏ��Ƀv���C���[���͂����Ă邩�� 0~�l�����őS���ɃA�N�Z�X�ł���
				statusArray[i] = *mBActors[i]->GetStatus();
			}
			mCharacterHealth.Update(mPlayerNum, statusArray);
			delete[] statusArray;
		}

		static int state = 0;
		if (isBehaviourEnable)
		{
			switch (state)
			{
			case 0: // �_���[�W�v�Z�A���o�J�n
			{
				BattleActor* targetActor = mBActors[mMoveActor->GetCommand()->GetTargetObjID()].get();
				int damage = CalcDamage(mMoveActor->GetStatus(), targetActor->GetStatus());
				targetActor->GetStatus()->HurtHP(damage);

				mProduction.Begin(mMoveActor->GetCommand()->GetBehaviour(), mMoveActor->GetObjID(), mMoveActor->GetCommand()->GetTargetObjID(), damage);
			}
			++state;
			//break;

			case 1: // ���o

				if (mProduction.Update(this))
				{
					// ���o���I�������
					mMoveActor->GetCommand()->BehaviourFinished();

					OrganizeActor();
					Result result = CheckBattleFinish();
					if (result == PLAYER_WIN || result == PLAYER_LOSE) // TODO : ���󕉂������̓��ʂȏ����͂Ȃ�(�����Ă������Ă��ꏏ)
					{
						BattleState::GetInstance().SetState(BattleState::State::RESULT);
						mHitEnemy->SetExist(false);
						mIsResult = true;

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
					else
					{
						DecideMoveActor();
						state = 0;

					}

				}
				break;
			}
		}

	}


}

void BattleActorManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBActors) ba->Render(view, projection, lightDir);

	if (!mIsResult)
	{
		mMoveActor->RenderCommand();    // MoveActor�̃R�}���hUI��\��
		mCharacterHealth.Render(false); // �L������HP��\��
		mProduction.Render();			// �U���̃_���[�W�Ƃ���\��
	}
}

void BattleActorManager::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBActors) ba->Render(shader, view, projection, lightDir);
}

int BattleActorManager::CalcDamage(const Status* deal, Status* take)
{
	int damage = deal->str / 2 - take->vit / 4; // ��b�_���[�W
	int sign = (rand() % 2 == 0) ? -1 : 1; // �U�ꕝ�̕���
	int width = damage / 16 + 1; // �_���[�W�̐U�ꕝ�̍ő�l
	damage = damage + (rand() % width * sign);

	if (damage < 0) damage = 0;

	return damage;
}

void BattleActorManager::PlayerCreateAndRegister(Player* pl)
{
	int objID = mBActors.size();
	mBActors.emplace_back(std::make_unique<PlayerBattle>(pl));
	mBActors.back()->SetObjID(objID);
	mAliveActorIDs[mBActors.back()->GetType()].push_back(objID);
}

void BattleActorManager::EnemyCreateAndRegister(Enemy* enm)
{
	int objID = mBActors.size();
	mBActors.emplace_back(std::make_unique<EnemyBattle>(enm));
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

void BattleActorManager::DecideMoveActor()
{
	if (mOrder.empty())
	{
		SortOrder();
	}

	while (true)
	{
		// ���Ԃ������L�������|����Ă���Ȃ疳��
		if (mOrder.front()->GetObjID() == -1)
		{
			mOrder.pop();
			if (mOrder.empty())
			{
				SortOrder();
				break;
			}
		}
		else break;
	}

	mMoveActor = mOrder.front();
	mOrder.pop();
}
