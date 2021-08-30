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
#include "PlayerManager.h"
#include "SceneManager.h"
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
	
	mTurnManager			= std::make_unique<TurnManager>();
	mBattleCharacterManager = std::make_unique<BattleCharacterManager>(plm, enemy);
	mSkybox					= std::make_unique<Skybox>();
	mTerrain				= std::make_shared<Terrain>(DataBase::TERRAIN_ID_START);

	// BGM決定
	switch (enemy->GetEnmType())
	{
	case StatusData::MOB:
		music = Music::BATTLE;
		result = Music::RESULT;
		break;

	case StatusData::BOSS:
		music = Music::BOSS_BATTLE;
		result = Music::BOSS_RESULT;
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

	AUDIO.MusicPlay((int)music);
}

void SceneBattle::Update()
{
	if (BattleState::GetInstance().CheckState(BattleState::State::RESULT))
	{
		if (mResult == PLAYER_LOSE)
		{

		}

		// フィールドに戻る
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			Fade::GetInstance().SetSceneImage(Fade::SPEED_SLOW);
			SceneManager::GetInstance().PopCurrentScene();
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
				// 戦闘後のステータスを更新
				for (int i = 0; i < mPlayerManager->GetNum(); ++i)
				{
					BattleCharacter* pl = mBattleCharacterManager->GetChara(i);
					pl->GetStatus()->ResetBuff(); // バフ消去
					Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(pl->GetCharaID(), *pl->GetStatus());
				}

				// ドロップアイテムをインベントリに加える
				for (auto& dropID : mBattleCharacterManager->GetDropItemIDs())
				{
					mPlayerManager->GetEquipmentInventory()->Push(dropID);
				}

				// BGMをリザルトのやつにする
				AUDIO.MusicStop((int)music);
				AUDIO.MusicPlay((int)result);
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
	mTurnManager->Render();
	mBattleCharacterManager->Render(view, projection, lightDir);
	Singleton<EffectManager>().GetInstance().Render(view, projection);
	mSceneTarget.Deactivate();

	mSceneTarget.Render(mPostEffect.get());
}

void SceneBattle::Release()
{
	Singleton<CameraManager>().GetInstance().Pop();
	BattleState::DestroyInst();

	// BGMストップ、フィールドBGM再生
	AUDIO.MusicStop((int)result);
	AUDIO.MusicStop((int)music);
	AUDIO.MusicPlay((int)Music::FIELD_REMAINS);
}
