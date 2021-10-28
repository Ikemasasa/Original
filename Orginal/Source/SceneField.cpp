#include "SceneField.h"

#include "lib/Audio.h"
#include "lib/Blend.h"
#include "lib/Renderer2D.h"
#include "lib/Skybox.h"

#include "CharacterManager.h"
#include "CollisionTerrain.h"
#include "CameraTPS.h"
#include "CameraManager.h"
#include "DataBase.h"
#include "Define.h"
#include "Enemy.h"
#include "EffectManager.h"
#include "Fade.h"
#include "GameManager.h"
#include "KeyGuide.h"
#include "Light.h"
#include "Player.h"
#include "SceneManager.h"
#include "SceneBattle.h"
#include "Singleton.h"
#include "Terrain.h"

SceneField::SceneField()
{
	CreateBaseAll();

	mCharaManager = std::make_unique<CharacterManager>();
	mSkybox = std::make_unique<Skybox>();
}

SceneField::~SceneField()
{
	CollisionTerrain::UnRegisterTerrainAll();
	SingletonFinalizer::Finalize();
}

void SceneField::Initialize()
{
	InitializeBaseAll();

	mTerrain = std::make_unique<Terrain>(DataBase::TERRAIN_ID_START);
	
	DataBase::Initialize();
	mSkybox->Initialize(L"Data/Image/environment.hdr");
	mTerrain->Initialize();
	mCharaManager->Initialize();

	Singleton<CameraManager>().GetInstance().Push(std::make_shared<CameraTPS>());
	Singleton<CameraManager>().GetInstance().Initialize(mCharaManager->GetMovePlayer());

	// フラグリセット
	GameManager::bossSubdueFlag = false;

	mIsLoadFinished = true;


	// BGM 再生開始
	Audio::MusicPlay((int)Music::FIELD_REMAINS);
}

void SceneField::Update()
{
	if (mTransBattleScene)
	{
		static float timer = 0.0f;
		if (timer >= TRANS_BATTLE_SCENE_TIME)
		{
			// シーン移行
			Fade::GetInstance().SetSceneImage(Fade::SPEED_VERY_SLOW, true);
			PlayerManager* pm = mCharaManager->GetPlayerManager();
			Enemy* enm = mCharaManager->IsHitEnemy();
			SceneManager::GetInstance().SetStackScene(std::make_unique<SceneBattle>(pm, enm));
			Audio::SoundPlay((int)Sound::ENEMY_HIT);

			mCharaManager->ResetHitEnemy();
			mTransBattleScene = false;
			timer = 0.0f;
		}
		else
		{
			timer += GameManager::elapsedTime;
		}
	}
	else
	{
		mCharaManager->Update();
		Singleton<CameraManager>().GetInstance().Update(mCharaManager->GetMovePlayer());

		mSkybox->SetEyePos(Singleton<CameraManager>().GetInstance().GetPos());

		Singleton<EffectManager>().GetInstance().Update();

		// 敵と当たったかチェック
		if (mCharaManager->IsHitEnemy())
		{
			mTransBattleScene = true;
		}

		// キーガイド
		KeyGuide::Instance().Add(KeyGuide::Y, L"メニュー");
	}
}

void SceneField::Render()
{
	Matrix view = Singleton<CameraManager>().GetInstance().GetView();
	Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();
	Vector4 lightDir = mLight.GetLightDir();

	// シャドウマップに書き込み
	const Shader* shader = mShadowMap->GetShader();
	mShadowMap->Activate(lightDir, mLight.GetLightPos());
	mTerrain->Render(shader, view, proj, lightDir);
	mCharaManager->Render(shader, view, proj, lightDir);
	mShadowMap->Deactivate();

	// GBufferに書き込み
	shader = mDeferredRenderer->GetGBufferShader();
	mDeferredRenderer->ActivateGBuffer();
	mSkybox->Render(view, proj);
	mTerrain->Render(shader, view, proj, lightDir);
	mCharaManager->Render(shader, view, proj, lightDir);
	mDeferredRenderer->DeactivateGBuffer();

	// テクスチャセット(GBuffer, 環境, 影
	mDeferredRenderer->SetGBufferTexture();
	mSkybox->SetEnvTexture(Define::ENVIRONMENT_TEXTURE_SLOT);
	mShadowMap->SetTexture(Define::SHADOWMAP_TEXTURE_SLOT);
	
	// ディファードのパラメータ用意(後々外部から入力したい
	SetDeferredParam();

	// ディファードレンダリング
	//// シーンターゲットに書き込み
	mPostEffectTarget->Activate();
	mDeferredRenderer->Render();
	// ブルーム作成、適用
	mBloom->Execute(mPostEffectTarget.get());
	mPostEffectTarget->Deactivate();

	// シーンターゲットに書き込み
	mSceneTarget->Activate();
	mPostEffectTarget->Render(mPostEffect.get());
	mCharaManager->RenderUI();
	mSceneTarget->Deactivate();

	// バックバッファに書き込み
	mSceneTarget->Render(nullptr);
}

void SceneField::Release()
{
	ReleaseBaseAll();

	Singleton<CameraManager>().GetInstance().Pop();

	DataBase::Release();
	mSkybox->Release();
}

void SceneField::SetDeferredParam()
{
	// ディファードのライト用意(後々外部から入力したい
	//// ライトクリア
	mDeferredRenderer->ClearLight();
	//// DirLight
	std::vector<DeferredRenderer::DirLight> dirLights;
	DeferredRenderer::DirLight dirLight;
	dirLight.dir = mLight.GetLightDir();
	dirLight.color = mLight.GetLightColor();
	dirLights.push_back(dirLight);
	mDeferredRenderer->SetDirLight(dirLights);

	//// PointLight
	std::vector<DeferredRenderer::PointLight> pointLights;
	DeferredRenderer::PointLight pointLight;
	pointLight.color = Vector4(0.95f, 0.75f, 0.55f, 1.0f);
	size_t num = mCharaManager->GetEnemyManager()->GetNum();
	for (size_t i = 0; i < num; ++i)
	{
		const Enemy* enm = mCharaManager->GetEnemyManager()->GetEnemy(i);

		// 座標、レンジ算出
		const float ADJUST_DIV = 1.2f;
		const AABB aabb = enm->GetLocalAABB();
		float range = (aabb.max.y - aabb.min.y) / ADJUST_DIV;
		pointLight.pos = Vector4(enm->GetPos(), range);
		pointLight.pos.y += aabb.max.y / ADJUST_DIV;
		pointLights.push_back(pointLight);
	}
	mDeferredRenderer->SetPointLight(pointLights);

	// 定数バッファ更新
	Vector3 eyePos = Singleton<CameraManager>().GetInstance().GetPos();
	mDeferredRenderer->SetCBPerFrame(eyePos);
}