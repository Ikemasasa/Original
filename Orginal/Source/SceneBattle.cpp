#include "SceneBattle.h"

#include "lib/Input.h"
#include "lib/Skybox.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraBattle.h"
#include "CameraManager.h"
#include "DataBase.h"
#include "Define.h"
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
	CreateBaseAll();

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
	InitializeBaseAll();

	Singleton<CameraManager>().GetInstance().Push(std::make_shared<CameraBattle>());

	mSkybox->Initialize(L"Data/Image/environment.hdr");
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
				for (size_t i = 0; i < mPlayerManager->GetNum(); ++i)
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
	Matrix view = Singleton<CameraManager>().GetInstance().GetView();
	Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();
	Vector4 lightDir = mLight.GetLightDir();

	// シャドウマップ
	const Shader* shader = mShadowMap->GetShader();
	mShadowMap->Activate(lightDir, mLight.GetLightPos());
	mTerrain->Render(shader, view, proj, lightDir);
	mBattleCharacterManager->Render(shader, view, proj, lightDir);
	mShadowMap->Deactivate();

	// GBufferに書き込み
	shader = mDeferredRenderer->GetGBufferShader();
	mDeferredRenderer->ActivateGBuffer();
	mSkybox->Render(view, proj);
	mTerrain->Render(shader, view, proj, lightDir);
	mBattleCharacterManager->Render(shader, view, proj, lightDir);
	mDeferredRenderer->DeactivateGBuffer();

	// テクスチャセット(Gbuffer, 環境, 影
	mDeferredRenderer->SetGBufferTexture();
	mSkybox->SetEnvTexture(Define::ENVIRONMENT_TEXTURE_SLOT);
	mShadowMap->SetTexture(Define::SHADOWMAP_TEXTURE_SLOT);

	// ディファードのライト用意(後々外部から入力したい
	//// ライトクリア
	mDeferredRenderer->ClearLight();
	//// DirLight
	std::vector<DeferredRenderer::DirLight> dirLights;
	DeferredRenderer::DirLight dirLight;
	dirLight.dir = lightDir;
	dirLight.color = mLight.GetLightColor();
	dirLights.push_back(dirLight);
	mDeferredRenderer->SetDirLight(dirLights);

	// SpotLight
	std::vector<DeferredRenderer::SpotLightArgu> spotLights;
	DeferredRenderer::SpotLightArgu sl;
	sl.pos = Vector3(0.0f, 30.0f, 0.0f);
	sl.target = Vector3(0.0f, 0.0f, 0.0f);
	sl.rgb = Vector3(0.15f, 0.15f, 0.15f);
	sl.inner = 30.0f;
	sl.outer = 45.0f;
	spotLights.emplace_back(sl);
	mDeferredRenderer->SetSpotLight(spotLights);


	Vector3 eyePos = Singleton<CameraManager>().GetInstance().GetPos();
	mDeferredRenderer->SetCBPerFrame(eyePos);

	// ディファードレンダリング
	//// ポストエフェクトターゲットに書き込み
	mPostEffectTarget->Activate();
	mDeferredRenderer->Render();
	Singleton<EffectManager>().GetInstance().Render(view, proj);
	// ブルーム作成、適用
	mBloom->Execute(mSceneTarget.get());
	mPostEffectTarget->Deactivate();

	// シーンターゲットに書き込み
	mSceneTarget->Activate();
	mPostEffectTarget->Render(mPostEffect.get());
	mTurnManager->Render();
	mBattleCharacterManager->RenderUI();
	mSelectOptions->Render(Vector2(OPTIONS_X, OPTIONS_Y));
	mSceneTarget->Deactivate();

	// バックバッファに結果を描画
	mSceneTarget->Render(nullptr);
}

void SceneBattle::Release()
{
	Singleton<CameraManager>().GetInstance().Pop();
	BattleState::DestroyInst();

	// フィールドBGM再生
	Audio::MusicPlay((int)Music::FIELD_REMAINS);
}
