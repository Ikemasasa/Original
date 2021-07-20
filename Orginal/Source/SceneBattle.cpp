#include "SceneBattle.h"

#include "lib/Audio.h"
#include "lib/Skybox.h"

#include "BattleActorManager.h"
#include "BattleState.h"
#include "CameraBattle.h"
#include "CameraManager.h"
#include "EffectManager.h"
#include "Light.h"
#include "SceneManager.h"
#include "Terrain.h"


SceneBattle::SceneBattle(PlayerManager* plm, Enemy* enemy)
{
	// SceneBaseの各種初期化
	mShadowMap.Initialize();
	mSceneTarget.Initialize();

	CreatePostEffectShader();

	// SceneBattleの各種初期化
	BattleState::CreateInst();
	BattleState::GetInstance().SetState(BattleState::State::COMMAND_SELECT);

	mBattleActorManager = std::make_unique<BattleActorManager>(plm, enemy);
	mSkybox = std::make_unique<Skybox>();

	mTerrain = std::make_shared<Terrain>(DataBase::TERRAIN_ID_START);
	mTerrain->Initialize();

	Singleton<CameraManager>().GetInstance().Push(std::make_shared<CameraBattle>());

	AUDIO.MusicPlay((int)Music::BATTLE);
}

SceneBattle::~SceneBattle()
{

}

void SceneBattle::Initialize()
{
	mBattleActorManager->Initialize();
	mSkybox->Initialize(L"Data/Image/sky.png");
}

void SceneBattle::Update()
{
	mBattleActorManager->Update();

	Singleton<CameraManager>().GetInstance().Update(mBattleActorManager->GetMoveActor());
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
	mBattleActorManager->Render(mShadowMap.GetShader(), view, projection, lightDir);
	mShadowMap.Deactivate(SHADOWMAP_TEXTURE_SLOT);

	// シーンターゲット
	mSceneTarget.Activate();
	mSkybox->Render(view, projection);
	mTerrain->Render(view, projection, lightDir);
	mBattleActorManager->Render(view, projection, lightDir);
	Singleton<EffectManager>().GetInstance().Render(view, projection);
	mSceneTarget.Deactivate();

	mSceneTarget.Render(mPostEffect.get());
}

void SceneBattle::Release()
{
	Singleton<CameraManager>().GetInstance().Pop();
	BattleState::DestroyInst();

	// リザルトBGMストップ、フィールドBGM再生
	AUDIO.MusicStop((int)Music::RESULT);
	AUDIO.MusicPlay((int)Music::FIELD_REMAINS);
}
