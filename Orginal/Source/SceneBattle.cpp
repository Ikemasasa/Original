#include "SceneBattle.h"

#include "lib/Input.h"
#include "lib/Skybox.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraBattle.h"
#include "CameraManager.h"
#include "DataBase.h"
#include "EffectManager.h"
#include "Enemy.h"
#include "Fade.h"
#include "Light.h"
#include "Player.h"
#include "PlayerManager.h"
#include "SelectOptions.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "Terrain.h"
#include "TurnManager.h"

SceneBattle::SceneBattle(PlayerManager* plm, Enemy* enemy)
{
	// SceneBaseの各種初期化
	mShadowMap.Initialize();
	mSceneTarget.Initialize();
	CreatePostEffectShader();

	// SceneBattleの各種初期化
	BattleState::CreateInst();
	BattleState::GetInstance().SetState(BattleState::State::BEGIN);

	// plm, enemy保存
	mPlayerManager = plm;
	mHitEnemy = enemy;

	mSelectOptions = std::make_unique<SelectOptions>();
	mTurnManager = std::make_unique<TurnManager>();
	mBattleCharacterManager = std::make_unique<BattleCharacterManager>(plm, enemy);
	mSkybox = std::make_unique<Skybox>();
	mTerrain = std::make_shared<Terrain>(DataBase::TERRAIN_ID_START);

	// BGM決定
	switch (enemy->GetEnmType())
	{
	case StatusData::MOB:
		mBattleMusic = Music::BATTLE;
		mResultMusic = Music::RESULT;
		break;

	case StatusData::BOSS:
		mBattleMusic = Music::BOSS_BATTLE;
		mResultMusic = Music::BOSS_RESULT;
		break;
	}
}

SceneBattle::~SceneBattle()
{

}

void SceneBattle::Initialize()
{
	Singleton<CameraManager>().GetInstance().Push(std::make_shared<CameraBattle>());

	mSkybox->Initialize(L"Data/Image/sky.png");
	mTerrain->Initialize();
	mBattleCharacterManager->Initialize();
	mTurnManager->Initialize(mBattleCharacterManager->GetBCharacters());

	mSelectOptions->Initialize();

	Audio::MusicPlay((int)mBattleMusic);
}

void SceneBattle::Update()
{
	if (BattleState::GetInstance().CheckState(BattleState::State::RESULT))
	{
		if (mResult == PLAYER_LOSE)
		{
			mSelectOptions->AddTitle(L"やりなおしますか？");
			mSelectOptions->AddOption(L"敵に当たる直前に戻る");
			mSelectOptions->AddOption(L"タイトルに戻る");
			mSelectOptions->Update();

			if (Input::GetButtonTrigger(0, Input::BUTTON::A))
			{
				Fade::GetInstance().Set(Fade::SPEED_SLOW);
			}

			if (Fade::GetInstance().IsFadeOutEnd())
			{
				int index = mSelectOptions->GetIndex();
				switch (index)
				{
				case 0: SceneManager::GetInstance().PopCurrentScene(); break;
				case 1: SceneManager::GetInstance().SetNextScene(std::make_unique<SceneTitle>()); break;
				}
			}

		}
		
		if (mResult == PLAYER_WIN)
		{
			// フィールドに戻る
			if (Input::GetButtonTrigger(0, Input::BUTTON::A))
			{
				Fade::GetInstance().SetSceneImage(Fade::SPEED_SLOW);
				SceneManager::GetInstance().PopCurrentScene();
			}
		}
	}

	mTurnManager->Update(mBattleCharacterManager.get());
	mBattleCharacterManager->Update(mTurnManager.get());

	// 1ターンが終了したら
	if (mTurnManager->IsTurnFinished())
	{
		// 死んでるアクターをチェック
		mBattleCharacterManager->OrganizeCharacter();

		// バトル終了かチェック
		mResult = mBattleCharacterManager->CheckBattleFinish();
		if (mResult != NONE)
		{
			// StateをResultにする
			BattleState::GetInstance().SetState(BattleState::State::RESULT);

			switch (mResult)
			{
			case PLAYER_WIN:
				// 当たった敵を消す
				mHitEnemy->SetExist(false);

				// 戦闘後のステータス、インベントリを更新
				for (int i = 0; i < mPlayerManager->GetNum(); ++i)
				{
					BattleCharacter* pl = mBattleCharacterManager->GetChara(i);
					pl->GetStatus()->ResetBuff(); // バフ消去
					StatusData::SetPLStatus(pl->GetCharaID(), *pl->GetStatus());

					mPlayerManager->GetPlayer(i)->SetInventory(pl->GetInventory());
				}

				// ドロップアイテムをインベントリに加える
				for (auto& dropID : mBattleCharacterManager->GetDropItemIDs())
				{
					const ItemData::BaseData* base = ItemData::GetBaseData(dropID);
					
					switch (base->type)
					{
					case ItemData::EQUIPMENT: mPlayerManager->GetEquipmentInventory()->Push(dropID); break;
					case ItemData::FIELD_USE: mPlayerManager->GetPlayer(0)->GetInventory()->Add(dropID); break;
					}
					
				}

				// BGMをリザルトのやつにする
				//Audio::MusicStop((int)mBattleMusic);
				Audio::MusicPlay((int)mResultMusic);
				break;

			case PLAYER_LOSE:
				break;
			}
		}
	}

	Singleton<CameraManager>().GetInstance().Update(mBattleCharacterManager->GetMoveChara()); 
	mSkybox->SetEyePos(Singleton<CameraManager>().GetInstance().GetPos());
	Singleton<EffectManager>().GetInstance().Update();
}

void SceneBattle::Render()
{
	DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
	DirectX::XMFLOAT4X4 projection = Singleton<CameraManager>().GetInstance().GetProj();
	DirectX::XMFLOAT4 lightDir = mLight.GetLightDir();

	// シャドウマップ
	mShadowMap.Activate(lightDir, SHADOWMAP_TEXTURE_SLOT);
	mTerrain->Render(mShadowMap.GetShader(), view, projection, lightDir);
	mBattleCharacterManager->Render(mShadowMap.GetShader(), view, projection, lightDir);
	mShadowMap.Deactivate(SHADOWMAP_TEXTURE_SLOT);

	// シーンターゲット
	mSceneTarget.Activate();
	mSkybox->Render(view, projection);
	mTerrain->Render(view, projection, lightDir);
	mBattleCharacterManager->Render(view, projection, lightDir);
	Singleton<EffectManager>().GetInstance().Render(view, projection);
	mTurnManager->Render();
	mSelectOptions->Render(Vector2(OPTIONS_X, OPTIONS_Y));
	mSceneTarget.Deactivate();

	mSceneTarget.Render(mPostEffect.get());
}

void SceneBattle::Release()
{
	Singleton<CameraManager>().GetInstance().Pop();
	BattleState::DestroyInst();

	// BGMストップ、フィールドBGM再生
	//Audio::MusicStop((int)mResultMusic);
	//Audio::MusicStop((int)mBattleMusic);
	Audio::MusicPlay((int)Music::FIELD_REMAINS);
}
