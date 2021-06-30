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

	// マップのキーは昇順にソートされてる
	for (auto& actor : mBActors)
	{
		agiOrder[actor->GetStatus()->agi].push_back(actor.get());
	}

	// 降順に代入したいからリバースイテレータ
	for (auto it = agiOrder.rbegin(); it != agiOrder.rend(); ++it)
	{
		// 2個以上ならその中からランダムで決める
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

	// プレイヤー登録
	mPlayerNum = player->GetNum();
	for (size_t i = 0; i < mPlayerNum; ++i)
	{
		PlayerCreateAndRegister(player->GetPlayer(i));
	}
	mIsResult = false;

	// 敵登録
	EnemyCreateAndRegister(enemy);

	SortOrder();
	mMoveActor = mOrder.front();
	mOrder.pop();
}

void BattleActorManager::Initialize()
{
	mCharacterHealth.Initialize(Vector2(HEALTH_PLATE_X, HEALTH_PLATE_Y));

	for (auto& ba : mBActors) ba->Initialize();

	// 座標設定
	{
		// PLAYER
		size_t size = mAliveActorIDs[Actor::Type::PLAYER].size();
		for (size_t i = 0; i < size; ++i)
		{
			// 今は1人なので仮
			Vector3 pos(0, 0, PLAYER_POS_Z);
			mBActors[mAliveActorIDs[Actor::Type::PLAYER][i]]->SetPos(pos);
			mBActors[mAliveActorIDs[Actor::Type::PLAYER][i]]->UpdateWorld();
		}

		// ENEMY
		size = mAliveActorIDs[Actor::Type::ENEMY].size();
		for (size_t i = 0; i < size; ++i)
		{
		// 今は1人なので仮
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
		// モーションの更新だけする
		for (auto& actor : mBActors) actor->UpdateWorld();

		// fieldに戻る
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
		// アクターのupdateをして、behaviourが決まれば演出、 次のアクターへ
		bool isBehaviourEnable = false;
		isBehaviourEnable = mMoveActor->Update(this);

		// MoveActor以外もモーションの更新をする
		for (auto& actor : mBActors)
		{
			if (mMoveActor == actor.get()) continue;
			actor->UpdateWorld();
		}

		// healthplate更新
		{
			Status* statusArray = new Status[mPlayerNum];
			for (int i = 0; i < mPlayerNum; ++i)
			{
				// BActorsは最初にプレイヤーがはいってるから 0~人数分で全員にアクセスできる
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
			case 0: // ダメージ計算、演出開始
			{
				BattleActor* targetActor = mBActors[mMoveActor->GetCommand()->GetTargetObjID()].get();
				int damage = CalcDamage(mMoveActor->GetStatus(), targetActor->GetStatus());
				targetActor->GetStatus()->HurtHP(damage);

				mProduction.Begin(mMoveActor->GetCommand()->GetBehaviour(), mMoveActor->GetObjID(), mMoveActor->GetCommand()->GetTargetObjID(), damage);
			}
			++state;
			//break;

			case 1: // 演出

				if (mProduction.Update(this))
				{
					// 演出が終わったら
					mMoveActor->GetCommand()->BehaviourFinished();

					OrganizeActor();
					Result result = CheckBattleFinish();
					if (result == PLAYER_WIN || result == PLAYER_LOSE) // TODO : 現状負けた時の特別な処理はない(勝っても負けても一緒)
					{
						BattleState::GetInstance().SetState(BattleState::State::RESULT);
						mHitEnemy->SetExist(false);
						mIsResult = true;

						// 戦闘後のステータスを更新
						for (int i = 0; i < mPlayerNum; ++i)
						{
							BattleActor* pl = mBActors[i].get();
							Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(pl->GetCharaID(), *pl->GetStatus());
						}

						// BGMをリザルトのやつにする
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
		mMoveActor->RenderCommand();    // MoveActorのコマンドUIを表示
		mCharacterHealth.Render(false); // キャラのHPを表示
		mProduction.Render();			// 攻撃のダメージとかを表示
	}
}

void BattleActorManager::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBActors) ba->Render(shader, view, projection, lightDir);
}

int BattleActorManager::CalcDamage(const Status* deal, Status* take)
{
	int damage = deal->str / 2 - take->vit / 4; // 基礎ダメージ
	int sign = (rand() % 2 == 0) ? -1 : 1; // 振れ幅の符号
	int width = damage / 16 + 1; // ダメージの振れ幅の最大値
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
	// mAliveActorIDsをチェックする

	Result ret = NONE;
	if (mAliveActorIDs[Actor::ENEMY].empty()) ret = PLAYER_WIN;
	if (mAliveActorIDs[Actor::PLAYER].empty()) ret = PLAYER_LOSE;

	return ret;
}

void BattleActorManager::OrganizeActor()
{
	// 体力が0になったアクタ―を整理する
	for (auto& ba : mBActors)
	{
		// 体力が1以上ならcontinue
		if (!ba->GetStatus()->IsDead()) continue;

		// 0以下なら mAliveActorIDs から消す
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
		// 順番が来たキャラが倒されているなら無視
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
