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
#include "Fade.h"
#include "PlayerBattle.h"
#include "PlayerManager.h"
#include "SceneBattle.h"
#include "SceneManager.h"
#include "Singleton.h"

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
	mCharacterHealth.Initialize(Vector2(HEALTH_BOARD_X, HEALTH_BOARD_Y));

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
	// リザルトじゃないなら
	if (BattleState::GetInstance().CheckState(BattleState::State::RESULT))
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

		// healthboard更新
		std::vector<Status> statusArray;
		for (int i = 0; i < mPlayerNum; ++i)
		{
			// BCharactersは最初にプレイヤーがはいってるから 0~人数分で全員にアクセスできる
			statusArray.push_back(*mBCharacters[i]->GetStatus());
		}
		mCharacterHealth.Update(statusArray);
	}
	else
	{
		// モーションの更新だけする
		for (auto& chara : mBCharacters) chara->UpdateWorld();
	}

	//if (mTurnManager.IsResult())
	{
		//// モーションの更新だけする
		//for (auto& chara : mBCharacters) chara->UpdateWorld();

		//// fieldに戻る
		//if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		//{
		//	if (Fade::GetInstance().SetSceneImage(Fade::SPEED_SLOW))
		//	{
		//		SceneManager::GetInstance().PopCurrentScene();
		//	}
		//}
	}
	//else
	{
		mMoveChara = turnManager->GetMoveChara();
		mMoveChara->Update(this);

		// MoveChara以外もワールド, モーションの更新をする
		for (auto& chara : mBCharacters)
		{
			if (mMoveChara == chara.get()) continue;
			chara->UpdateWorld();
		}

		// healthboard更新
		std::vector<Status> statusArray;
		for (int i = 0; i < mPlayerNum; ++i)
		{
			// BCharactersは最初にプレイヤーがはいってるから 0~人数分で全員にアクセスできる
			statusArray.push_back(*mBCharacters[i]->GetStatus());
		}
		mCharacterHealth.Update(statusArray);

		// 1ターンが終了したら
		//if (mTurnManager.IsTurnFinished())
		//{
		//	// 死んでるアクターをチェック
		//	OrganizeCharacter();

		//	// バトル終了かチェック
		//	Result result = CheckBattleFinish();
		//	if (result == PLAYER_WIN) // TODO : 現状負けた時の特別な処理はない(勝っても負けても一緒)
		//	{
		//		BattleState::GetInstance().SetState(BattleState::State::RESULT);
		//		mHitEnemy->SetExist(false);
		//		
		//		// リザルトに移行
		//		mTurnManager.ToResult();

		//		// 戦闘後のステータスを更新
		//		for (int i = 0; i < mPlayerNum; ++i)
		//		{
		//			BattleCharacter* pl = mBCharacters[i].get();
		//			pl->GetStatus()->ResetBuff();
		//			Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(pl->GetCharaID(), *pl->GetStatus());
		//		}

		//		// ドロップアイテムをインベントリに加える
		//		for (auto& dropID : mDropItemIDs)
		//		{
		//			mPlayerManager->GetEquipmentInventory()->Push(dropID);
		//		}

		//		// BGMをリザルトのやつにする
		//		AUDIO.MusicStop((int)SceneBattle::music);
		//		AUDIO.MusicPlay((int)SceneBattle::result);
		//	}
		//	else
		//	{
		//		// とりあえずタイトルに戻す
		//	}
	}

	mDropItemShower.Update();
}

void BattleCharacterManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBCharacters) ba->Render(view, projection, lightDir);

	// リザルトじゃないならUIﾄｶを表示
	if (BattleState::GetInstance().CheckState(BattleState::State::RESULT))
	{
		mMoveChara->RenderCommand();    // MoveCharaのコマンドUIを表示
		mCharacterHealth.Render(false); // キャラのHPを表示
	}

	mDropItemShower.Render();
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
	for (auto& ba : mBCharacters)
	{
		// 体力が1以上ならcontinue, すでにexistがfalseなら
		if (!ba->GetStatus()->IsDead()) continue;

		// 敵ならドロップのチェックをする
		if (ba->GetType() == Character::ENEMY)
		{
			int dropItemID = 0;
			Singleton<DataBase>().GetInstance().GetDropData()->DecideDropItem(ba->GetCharaID(), &dropItemID);
			if (dropItemID != -1)
			{
				mDropItemIDs.push_back(dropItemID);
				mDropItemShower.Add(dropItemID, ba->GetPos());
			}
		}

		// 0以下なら mAliveCharaIDs から消す
		auto& ids = mAliveObjIDs[ba->GetType()];
		for (auto it = ids.begin(); it != ids.end(); ++it)
		{
			// 敵ならドロップのチェックをする
			if (*it == ba->GetObjID())
			{
				ids.erase(it);
				break;
			}
		}
	}
}