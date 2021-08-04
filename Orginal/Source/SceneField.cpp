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
#include "EffectManager.h"
#include "Light.h"
#include "Player.h"
#include "SceneManager.h"
#include "SceneBattle.h"
#include "Singleton.h"
#include "Terrain.h"

SceneField::SceneField()
{
	mRamp.Load(L"Data/Image/Ramp.png");
	mRamp.Set(15);

	CreatePostEffectShader();

	mSceneTarget.Initialize();
	mShadowMap.Initialize();

	Singleton<DataBase>().GetInstance();
	Singleton<CameraManager>().GetInstance().Push(std::make_shared<CameraTPS>());

	mCharaManager = std::make_unique<CharacterManager>();
	mSkybox		  = std::make_unique<Skybox>();

	mTerrain = std::make_unique<Terrain>(DataBase::TERRAIN_ID_START);
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
	mTerrain->Initialize();
	mCharaManager->Initialize();
}

void SceneField::Update()
{
	mCharaManager->Update();
	Singleton<CameraManager>().GetInstance().Update(mCharaManager->GetMovePlayer());

	mSkybox->SetEyePos(Singleton<CameraManager>().GetInstance().GetPos());

	Singleton<EffectManager>().GetInstance().Update();
}

void SceneField::Render()
{
	DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
	DirectX::XMFLOAT4X4 proj = Singleton<CameraManager>().GetInstance().GetProj();
	DirectX::XMFLOAT4 lightDir = mLight.GetLightDir();

	// シャドウマップ
	mShadowMap.Activate(lightDir, SHADOWMAP_TEXTURE_SLOT);
	mTerrain->Render(mShadowMap.GetShader(), view, proj, lightDir);
	mCharaManager->Render(mShadowMap.GetShader(), view, proj, lightDir);
	mShadowMap.Deactivate(SHADOWMAP_TEXTURE_SLOT);

	// シーンターゲットに書き込み
	mSceneTarget.Activate();
	mSkybox->Render(view, proj);
	mTerrain->Render(view, proj, lightDir);
	mCharaManager->Render(view, proj, lightDir);
	mSceneTarget.Deactivate();

	mSceneTarget.Render(mPostEffect.get());
}

void SceneField::Release()
{
	AUDIO.MusicStop((int)Music::FIELD_REMAINS);

	mSkybox->Release();
	mPostEffect->UnLoad();
}