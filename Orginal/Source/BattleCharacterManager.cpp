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
	mCharacterHealth.Initialize(Vector2(HEALTH_PLATE_X, HEALTH_PLATE_Y));
	mTurnManager.Initialize(mBCharacters);

	for (auto& ba : mBCharacters) ba->Initialize();
	// 座標設定
	{
		// TODO: モデルの大きさを考慮していないから、モデルによったらバグる可能性あり

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
		// モーションの更新だけする
		for (auto& chara : mBCharacters) chara->UpdateWorld();

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
		BattleCharacter* moveChara = mTurnManager.GetMoveChara();
		moveChara->Update(this);

		// MoveChara以外もワールド, モーションの更新をする
		for (auto& chara : mBCharacters)
		{
			if (moveChara == chara.get()) continue;
			chara->UpdateWorld();
		}

		// healthplate更新
		{
			std::vector<Status> statusArray;
			for (int i = 0; i < mPlayerNum; ++i)
			{
				// BCharactersは最初にプレイヤーがはいってるから 0~人数分で全員にアクセスできる
				statusArray.push_back(*mBCharacters[i]->GetStatus());
			}
			mCharacterHealth.Update(statusArray);
		}


		mTurnManager.Update(this);



		// 1ターンが終了したら
		if (mTurnManager.IsTurnFinished())
		{
			// 死んでるアクターをチェック
			OrganizeCharacter();

			// バトル終了かチェック
			Result result = CheckBattleFinish();
			if (result == PLAYER_WIN) // TODO : 現状負けた時の特別な処理はない(勝っても負けても一緒)
			{
				BattleState::GetInstance().SetState(BattleState::State::RESULT);
				mHitEnemy->SetExist(false);
				
				// リザルトに移行
				mTurnManager.ToResult();

				// 戦闘後のステータスを更新
				for (int i = 0; i < mPlayerNum; ++i)
				{
					BattleCharacter* pl = mBCharacters[i].get();
					Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(pl->GetCharaID(), *pl->GetStatus());
				}

				// ドロップアイテムをインベントリに加える
				for (auto& dropID : mDropItemIDs)
				{
					mPlayerManager->GetEquipmentInventory()->Push(dropID);
				}

				// BGMをリザルトのやつにする
				AUDIO.MusicStop((int)Music::BATTLE);
				AUDIO.MusicPlay((int)Music::RESULT);
			}
			else
			{
				// とりあえずタイトルに戻す
			}
		}
	}


}

void BattleCharacterManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	for (auto& ba : mBCharacters) ba->Render(view, projection, lightDir);

	// リザルトじゃないならUIﾄｶを表示
	if (!mTurnManager.IsResult())
	{
		mTurnManager.GetMoveChara()->RenderCommand();    // MoveCharaのコマンドUIを表示
		mCharacterHealth.Render(false); // キャラのHPを表示
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
	// mAliveCharaIDsをチェックする
	Result ret = NONE;
	if (mAliveCharaIDs[Character::ENEMY].empty())  ret = PLAYER_WIN;
	if (mAliveCharaIDs[Character::PLAYER].empty()) ret = PLAYER_LOSE;

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
			mDropItemIDs.push_back(dropItemID);
		}

		// 0以下なら mAliveCharaIDs から消す
		auto& ids = mAliveCharaIDs[ba->GetType()];
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