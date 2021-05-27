#include "BattleActorManager.h"

#include <map>
#include <random>

#include "lib/Math.h"

#include "BattleState.h"
#include "EnemyBattle.h"
#include "PlayerBattle.h"

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

		for (int i = dist - 1; i > 0; ++i)
		{
			int target = std::uniform_int_distribution<int>(i, dist - 1)(engine);
			if (i != target) std::swap(temp[i], temp[target]);
		}

		return temp;
	};

	std::map<int, std::vector<std::shared_ptr<BattleActor>>>  agiOrder;

	// マップのキーは昇順にソートされてる
	for (auto& actor : mBActors)
	{
		agiOrder[actor->GetStatus()->agi].emplace_back(actor);
	}

	// 降順に代入したいからリバースイテレータ
	for (auto it = agiOrder.rbegin(); it != agiOrder.rend(); ++it)
	{
		// 2個以上ならその中からランダムで決める
		if (it->second.size() > 1)
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

BattleActorManager::BattleActorManager(const std::shared_ptr<Player>& player, const std::shared_ptr<Enemy>& enemy)
{
	// 一緒くたにする
	CreateAndRegister(player);
	CreateAndRegister(enemy);

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

	for (auto& actor : mBActors)
	{
		if (mMoveActor == actor) continue;
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
}

void BattleActorManager::CreateAndRegister(const std::shared_ptr<Actor>& actor)
{
	std::shared_ptr<BattleActor> ba = nullptr;
	Actor::Type type = actor->GetType();
	switch (type)
	{
	case Actor::PLAYER: ba = std::make_shared<PlayerBattle>(actor); break;
	case Actor::ENEMY:  ba = std::make_shared<EnemyBattle>(actor);  break;
	}

	int objID = mBActors.size();

	ba->SetObjID(objID);
	mObjectIDs[ba->GetType()].push_back(objID);

	mBActors.emplace_back(ba);
}

int BattleActorManager::CalcDamage(const Status* deal, Status* take)
{
	int damage = deal->str / 2 - take->vit / 4;
	int sign = (rand() % 2 == 0) ? -1 : 1;
	int width = damage / 16 + 1; // ダメージの振れ幅
	damage = damage + (rand() % width * sign);

	take->hp -= Math::Max(0, damage);
	if (take->hp <= 0) take->hp = 0;

	return damage;
}

bool BattleActorManager::CheckBattleFinish()
{
	for (auto& ba : mBActors)
	{
		// 体力が0以下なら exist = false, objIDに-1を登録
		if (ba->GetStatus()->hp <= 0)
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
	mMoveActor = mOrder.front();
	mOrder.pop();

	if (mOrder.empty())
	{
		SortOrder();
	}
}
