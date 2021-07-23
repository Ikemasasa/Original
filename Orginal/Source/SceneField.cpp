#include "SceneField.h"

#include "lib/Audio.h"
#include "lib/Blend.h"
#include "lib/Renderer2D.h"
#include "lib/Skybox.h"

#include "ActorManager.h"
#include "CollisionTerrain.h"
#include "CameraTPS.h"
#include "CameraManager.h"
#include "Define.h"
#include "EffectManager.h"
#include "Light.h"
#include "SceneManager.h"
#include "SceneBattle.h"
#include "Singleton.h"

SceneField::SceneField()
{
	// Gバッファ作成
	InitializeGBuffer();

	mRamp.Load(L"Data/Image/Ramp.png");
	mRamp.Set(15);

	CreatePostEffectShader();

	mSceneTarget.Initialize();
	mShadowMap.Initialize();

	Singleton<DataBase>().GetInstance();
	Singleton<CameraManager>().GetInstance().Push(std::make_shared<CameraTPS>());

	mActorManager = std::make_unique<ActorManager>();
	mSkybox		  = std::make_unique<Skybox>();
}

SceneField::~SceneField()
{
	CollisionTerrain::UnRegisterTerrainAll();
	SingletonFinalizer::Finalize();
}

void SceneField::Initialize()
{
	// BGM 再生開始
	AUDIO.MusicPlay((int)Music::FIELD_REMAINS);

	// ライト設定
	{
		Vector4 lightDir(0.5f, -0.5f, -1.0f, 1.0f);
		mLight.SetLightDir(lightDir);

		Vector4 lightColor(1, 1, 1, 1);
		mLight.SetLightColor(lightColor);

		mLight.CreateConstBuffer();
		mLight.UpdateConstBuffer();
		mLight.SetConstBuffer(1);
	}

	mSkybox->Initialize(L"Data/Image/sky.png");
	mActorManager->Initialize();
}

void SceneField::Update()
{
	mActorManager->Update();
	Singleton<CameraManager>().GetInstance().Update(mActorManager->GetMovePlayer());

	mSkybox->SetEyePos(Singleton<CameraManager>().GetInstance().GetPos());

	Singleton<EffectManager>().GetInstance().Update();
}

void SceneField::Render()
{
	DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
	DirectX::XMFLOAT4X4 proj = Singleton<CameraManager>().GetInstance().GetProj();
	DirectX::XMFLOAT4 lightDir = mLight.GetLightDir();

	mShadowMap.Activate(lightDir, SHADOWMAP_TEXTURE_SLOT);
	mActorManager->Render(mShadowMap.GetShader(), view, proj, lightDir);
	mShadowMap.Deactivate(SHADOWMAP_TEXTURE_SLOT);

	// シーンターゲットに書き込み
	mSceneTarget.Activate();
	mSkybox->Render(view, proj);
	mActorManager->Render(view, proj, lightDir);
	mSceneTarget.Deactivate();

	mSceneTarget.Render(mPostEffect.get());
}

void SceneField::Release()
{
	AUDIO.MusicStop((int)Music::FIELD_REMAINS);

	mSkybox->Release();
	mPostEffect->UnLoad();
}