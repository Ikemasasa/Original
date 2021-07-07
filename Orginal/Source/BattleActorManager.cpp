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

	// プレイヤー登録
	mPlayerNum = player->GetNum();
	for (size_t i = 0; i < mPlayerNum; ++i)
	{
		PlayerCreateAndRegister(player->GetPlayer(i));
	}

	// 敵登録
	EnemyCreateAndRegister(enemy);
}

void BattleActorManager::Initialize()
{
	mCharacterHealth.Initialize(Vector2(HEALTH_PLATE_X, HEALTH_PLATE_Y));
	mTurnManager.Initialize(mBActors);

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
}

void BattleActorManager::Update()
{
	if (mTurnManager.IsResult())
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
		BattleActor* moveActor = mTurnManager.GetMoveActor();
		moveActor->Update(this);

		// MoveActor以外もワールド, モーションの更新をする
		for (auto& actor : mBActors)
		{
			if (moveActor == actor.get()) continue;
			actor->UpdateWorld();
		}

		// healthplate更新
		{
			std::vector<Status> statusArray;
			for (int i = 0; i < mPlayerNum; ++i)
			{
				// BActorsは最初にプレイヤーがはいってるから 0~人数分で全員にアクセスできる
				statusArray.push_back(*mBActors[i]->GetStatus());
			}
			mCharacterHealth.Update(statusArray);
		}


		mTurnManager.Update(this);

		// 1ターンが終了したら
		if (mTurnManager.IsTurnFinished())
		{
			// 死んでるアクターをチェック
			OrganizeActor();

			// バトル終了かチェック
			Result result = CheckBattleFinish();
			if (result == PLAYER_WIN || result == PLAYER_LOSE) // TODO : 現状負けた時の特別な処理はない(勝っても負けても一緒)
			{
				BattleState::GetInstance().SetState(BattleState::State::RESULT);
				mHitEnemy->SetExist(false);
				
				// リザルトに移行
				mTurnManager.ToResult();

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
		}
	}


}

void BattleActorManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBActors) ba->Render(view, projection, lightDir);

	// リザルトじゃないならUIﾄｶを表示
	if (!mTurnManager.IsResult())
	{
		mTurnManager.GetMoveActor()->RenderCommand();    // MoveActorのコマンドUIを表示
		mCharacterHealth.Render(false); // キャラのHPを表示
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

//void BattleActorManager::DecideMoveActor()
//{
//	if (mOrder.empty())
//	{
//		SortOrder();
//	}
//
//	while (true)
//	{
//		// 順番が来たキャラが倒されているなら無視
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
