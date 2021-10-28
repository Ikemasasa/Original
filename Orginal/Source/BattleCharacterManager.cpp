#include "BattleCharacterManager.h"

#include <map>
#include <random>

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleState.h"
#include "BossEnemyBattle.h"
#include "DropData.h"
#include "EffectManager.h"
#include "Enemy.h"
#include "EnemyBattle.h"
#include "PlayerBattle.h"
#include "PlayerManager.h"
#include "SceneBattle.h"
#include "SceneManager.h"
#include "TurnManager.h"

BattleCharacterManager::BattleCharacterManager(PlayerManager* pm, Enemy* enemy)
{
	// プレイヤー登録
	mPlayerNum = pm->GetNum();
	for (int i = 0; i < mPlayerNum; ++i)
	{
		PlayerCreateAndRegister(pm->GetPlayer(i));
	}

	// 敵登録
	EnemyCreateAndRegister(enemy);
}

void BattleCharacterManager::Initialize()
{
	for (auto& ba : mBCharacters) ba->Initialize();
	// 座標設定
	{
		// TODO: モデルの大きさを考慮していないから、モデルによったらバグる可能性あり

		// PLAYER
		size_t size = mAliveObjIDs[Character::Type::PLAYER].size();
		float offsetX = (POS_MAX_X - POS_MIN_X) / (size + 1);
		for (size_t i = 0; i < size; ++i)
		{
			Vector3 pos(POS_MIN_X + offsetX * (i + 1), 0, PLAYER_POS_Z);
			mBCharacters[mAliveObjIDs[Character::Type::PLAYER][i]]->SetPos(pos);
			mBCharacters[mAliveObjIDs[Character::Type::PLAYER][i]]->UpdateWorld();
		}

		// ENEMY
		size = mAliveObjIDs[Character::Type::ENEMY].size();
		offsetX = (POS_MAX_X - POS_MIN_X) / (size + 1);
		for (size_t i = 0; i < size; ++i)
		{
			Vector3 pos(POS_MIN_X + offsetX * (i + 1), 0, ENEMY_POS_Z);
			mBCharacters[mAliveObjIDs[Character::Type::ENEMY][i]]->SetPos(pos);
			mBCharacters[mAliveObjIDs[Character::Type::ENEMY][i]]->UpdateWorld();
		}

	}
}

void BattleCharacterManager::Update(const TurnManager* turnManager)
{
	// リザルトなら
	if (BattleState::GetInstance().CheckState(BattleState::State::RESULT))
	{
		// モーションの更新だけする
		for (auto& chara : mBCharacters) chara->UpdateWorld();
	}
	else
	{
		// 現在のターンのキャラを取得
		mMoveChara = turnManager->GetMoveChara();
		mMoveChara->Update(this);

		// MoveChara以外もワールド, モーションの更新をする
		for (auto& chara : mBCharacters)
		{
			if (mMoveChara == chara.get()) continue;
			chara->UpdateWorld();
		}
	}

	mDropItemShower.Update();
}

void BattleCharacterManager::Render(const Shader* shader, const Matrix& view, const Matrix& projection, const Vector4& lightDir)
{
	for (auto& ba : mBCharacters) ba->Render(shader, view, projection, lightDir);
}

void BattleCharacterManager::RenderUI()
{
	// リザルトじゃないならUIﾄｶを表示
	if (!BattleState::GetInstance().CheckState(BattleState::State::RESULT))
	{
		mMoveChara->RenderCommand();    // MoveCharaのコマンドUIを表示
	}

	mDropItemShower.Render();
}

void BattleCharacterManager::PlayerCreateAndRegister(Player* pl)
{
	int objID = mBCharacters.size();
	mBCharacters.emplace_back(std::make_shared<PlayerBattle>(pl));
	mBCharacters.back()->SetObjID(objID);
	mAliveObjIDs[mBCharacters.back()->GetType()].push_back(objID);
}

void BattleCharacterManager::EnemyCreateAndRegister(Enemy* enm)
{
	int objID = mBCharacters.size();
	switch (enm->GetEnmType())
	{
	case StatusData::EnemyType::MOB:  mBCharacters.emplace_back(std::make_shared<EnemyBattle>(enm)); break;
	case StatusData::EnemyType::BOSS: mBCharacters.emplace_back(std::make_shared<BossEnemyBattle>(enm)); break;
	}

	mBCharacters.back()->SetObjID(objID);
	mAliveObjIDs[mBCharacters.back()->GetType()].push_back(objID);
}

SceneBattle::Result BattleCharacterManager::CheckBattleFinish()
{
	// mAliveCharaIDsをチェックする
	SceneBattle::Result ret = SceneBattle::NONE;
	if (mAliveObjIDs[Character::ENEMY].empty())  ret = SceneBattle::PLAYER_WIN;
	if (mAliveObjIDs[Character::PLAYER].empty()) ret = SceneBattle::PLAYER_LOSE;

	return ret;
}

void BattleCharacterManager::OrganizeCharacter()
{
	// 体力が0になったアクタ―を整理する
	std::list<int> removeCharaObjIDs;

	for (auto& typeObjIDs : mAliveObjIDs)
	{
		for (auto& objID : typeObjIDs)
		{
			// キャラを取得
			BattleCharacter* ba = mBCharacters[objID].get();
			
			// 生きてるならcontinue;
			if (!ba->GetStatus()->IsDead()) continue;

			// 敵ならドロップチェック
			if (ba->GetType() == Character::ENEMY)
			{
				int dropItemID = 0;
				DropData::DecideDropItem(ba->GetCharaID(), &dropItemID);
				if (dropItemID != -1)
				{
					mDropItemIDs.push_back(dropItemID);
					mDropItemShower.Add(dropItemID, ba->GetPos());
				}
			}

			// 削除リストに登録
			removeCharaObjIDs.push_back(objID);
		}
	}

	// 誰も死んでないならreturn
	if (removeCharaObjIDs.empty()) return;

	// mAliveObjIDsから削除する
	for (auto& typeObjIDs : mAliveObjIDs)
	{
		for (auto it = typeObjIDs.begin(); it != typeObjIDs.end();)
		{
			if (removeCharaObjIDs.empty()) break;

			// removeCharaObjIDsは昇順に入ってるから先頭から消していくだけで行ける
			if (*it == removeCharaObjIDs.front())
			{
				removeCharaObjIDs.erase(removeCharaObjIDs.begin());
				it = typeObjIDs.erase(it);
			}
			else ++it;
		}
	}
}