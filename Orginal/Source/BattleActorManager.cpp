#include "BattleActorManager.h"

#include <map>
#include <random>

#include "lib/Math.h"

#include "BattleState.h"
#include "EnemyBattle.h"
#include "PlayerBattle.h"
#include "PlayerManager.h"

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
	for (size_t i = 0; i < player->GetNum(); ++i)
	{
		PlayerCreateAndRegister(player->GetPlayer(i));
	}

	EnemyCreateAndRegister(enemy);

	SortOrder();
	mMoveActor = mOrder.front();
	mOrder.pop();
}

void BattleActorManager::Initialize()
{
	for (auto& ba : mBActors) ba->Initialize();

	// 座標設定
	{
		// PLAYER
		size_t size = mObjectIDs[Actor::Type::PLAYER].size();
		for (size_t i = 0; i < size; ++i)
		{
			// 今は1人なので仮
			Vector3 pos(0, 0, PLAYER_POS_Z);
			mBActors[mObjectIDs[Actor::Type::PLAYER][i]]->SetPos(pos);
			mBActors[mObjectIDs[Actor::Type::PLAYER][i]]->UpdateWorld();
		}

		// ENEMY
		size = mObjectIDs[Actor::Type::ENEMY].size();
		for (size_t i = 0; i < size; ++i)
		{
			// 今は1人なので仮
			Vector3 pos(0, 0, ENEMY_POS_Z);
			mBActors[mObjectIDs[Actor::Type::ENEMY][i]]->SetPos(pos);
			mBActors[mObjectIDs[Actor::Type::ENEMY][i]]->UpdateWorld();
		}

	}

}

void BattleActorManager::Update()
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


	static int state = 0;
	if (isBehaviourEnable)
	{
		switch (state)
		{
		case 0: // 初期設定
			BattleState::GetInstance().SetState(BattleState::State::ATTACK);
			++state;
			// break;

		case 1: //計算
			{
				int damage = CalcDamage(mMoveActor->GetStatus(), mBActors[mMoveActor->GetCommand()->GetTargetObjID()]->GetStatus());
				mProduction.Begin(mMoveActor->GetCommand()->GetBehaviour(), mMoveActor->GetObjID(), mMoveActor->GetCommand()->GetTargetObjID(), damage);
			}
			++state;
			//break;

		case 2:

			if (mProduction.Update(this))
			{
				// 演出が終わったら
				state = 0;
				mMoveActor->GetCommand()->BehaviourFinished();

				CheckBattleFinish();
				DecideMoveActor();
				BattleState::GetInstance().SetState(BattleState::State::COMMAND_SELECT);
			}


		}
	}


}

void BattleActorManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBActors) ba->Render(view, projection, lightDir);
	for (auto& ba : mBActors) ba->RenderCommand();
}

int BattleActorManager::CalcDamage(const Status* deal, Status* take)
{
	int damage = deal->str / 2 - take->vit / 4; // 基礎ダメージ
	int sign = (rand() % 2 == 0) ? -1 : 1; // 振れ幅の符号
	int width = damage / 16 + 1; // ダメージの振れ幅の最大値
	damage = damage + (rand() % width * sign);

	take->HurtHP(damage);
	return damage;
}

void BattleActorManager::PlayerCreateAndRegister(Player* pl)
{
	int objID = mBActors.size();
	mBActors.emplace_back(std::make_unique<PlayerBattle>(pl));
	mBActors.back()->SetObjID(objID);
	mObjectIDs[mBActors.back()->GetType()].push_back(objID);
}

void BattleActorManager::EnemyCreateAndRegister(Enemy* enm)
{
	int objID = mBActors.size();
	mBActors.emplace_back(std::make_unique<EnemyBattle>(enm));
	mBActors.back()->SetObjID(objID);
	mObjectIDs[mBActors.back()->GetType()].push_back(objID);
}

bool BattleActorManager::CheckBattleFinish()
{
	for (auto& ba : mBActors)
	{
		// 体力が0以下なら exist = false, objIDに-1を登録
		if (ba->GetStatus()->IsDead())
		{
			ba->SetExist(false);
			for (auto& objID : mObjectIDs[ba->GetType()])
			{
				if (ba->GetObjID() == objID)
				{
					objID = -1;
				}
			}
		}
	}

	bool isAllDead = true;
	for (auto& objID : mObjectIDs[Actor::Type::ENEMY])
	{
		if (objID != -1) isAllDead = false;
	}

	return isAllDead;
}

void BattleActorManager::DecideMoveActor()
{
	while (true)
	{
		// 順番が来たキャラが倒されているなら無視
		if (mOrder.front()->GetObjID() == -1) mOrder.pop();
		else break;
	}

	if (mOrder.empty())
	{
		SortOrder();
	}

	mMoveActor = mOrder.front();
	mOrder.pop();
}
