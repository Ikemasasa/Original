#include "SceneField.h"

#include "lib/Skybox.h"
#include "lib/Input.h"
#include "lib/Font.h"

#include "ActorManager.h"
#include "CollisionTerrain.h"
#include "CameraTPS.h"
#include "CameraManager.h"
#include "DataBase.h"
#include "Define.h"
#include "EffectManager.h"
#include "Enemy.h"
#include "Fade.h"
#include "MeshManager.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "SceneBattle.h"
#include "Singleton.h"
#include "Terrain.h"

SceneField::SceneField()
{
	mRamp.Load(L"Data/Image/Ramp.png");
	mRamp.Set(15);

	mPostEffect = std::make_unique<Shader>();
	mPostEffect->Load(L"Shaders/PostEffect.fx", "VSMain", "PSMain");

	mSceneTarget.Initialize();
	mShadowMap.Initialize();

	Singleton<DataBase>().GetInstance();
	Singleton<CameraManager>().GetInstance().Push(std::make_shared<CameraTPS>());

	mLightDirection = DirectX::XMFLOAT4(0.5f, -0.5f, -1.0f, 1.0f);

	mActorManager = std::make_unique<ActorManager>();
	mSkybox		  = std::make_unique<Skybox>(L"Data/Image/sky.png");

}

SceneField::~SceneField()
{
	CollisionTerrain::UnRegisterTerrainAll();
	SingletonFinalizer::Finalize();
}

void SceneField::Initialize()
{
	mActorManager->Initialize();
}

void SceneField::Update()
{
	mActorManager->Update();
	Singleton<CameraManager>().GetInstance().Update(mActorManager->GetPlayerManager()->GetMovePlayer());

	mSkybox->SetEyePos(Singleton<CameraManager>().GetInstance().GetPos());

	Singleton<EffectManager>().GetInstance().Update();
}

void SceneField::Render()
{
	DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
	DirectX::XMFLOAT4X4 projection = Singleton<CameraManager>().GetInstance().GetProj();

	mShadowMap.Activate(mLightDirection);
	mActorManager->Render(mShadowMap.GetShader(), view, projection, mLightDirection);
	mShadowMap.Deactivate();

	mSceneTarget.Activate();
	mSkybox->Render(view, projection);
	mActorManager->Render(view, projection, mLightDirection);
	Singleton<EffectManager>().GetInstance().Render(view, projection);
	mSceneTarget.Deactivate();

	mSceneTarget.Render(mPostEffect.get());
}

void SceneField::Release()
{
	mPostEffect->UnLoad();
}