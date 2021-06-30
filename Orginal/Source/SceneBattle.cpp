#include "SceneBattle.h"

#include "lib/Input.h"
#include "lib/Skybox.h"

#include "BattleActorManager.h"
#include "BattleState.h"
#include "CameraBattle.h"
#include "CameraManager.h"
#include "EnemyBattle.h"
#include "EffectManager.h"
#include "Fade.h"
#include "FrontendBattle.h"
#include "PlayerBattle.h"
#include "SceneManager.h"
#include "Terrain.h"


SceneBattle::SceneBattle(PlayerManager* plm, Enemy* enemy)
{
	// SceneBase�̊e�평����
	mShadowMap.Initialize();
	mSceneTarget.Initialize();

	mPostEffect = std::make_unique<Shader>();
	mPostEffect->Load(L"Shaders/PostEffect.fx", "VSMain", "PSMain");

	// SceneBattle�̊e�평����
	BattleState::CreateInst();
	BattleState::GetInstance().SetState(BattleState::State::COMMAND_SELECT);

	mBattleActorManager = std::make_unique<BattleActorManager>(plm, enemy);
	mSkybox = std::make_unique<Skybox>(L"Data/Image/sky.png");

	mTerrain = std::make_shared<Terrain>(DataBase::TERRAIN_ID_START);
	mTerrain->Initialize();

	mLightDir = DirectX::XMFLOAT4(0.5f, -0.5f, -1.0f, 1.0f);

	Singleton<CameraManager>().GetInstance().Push(std::make_shared<CameraBattle>());
}

SceneBattle::~SceneBattle()
{

}

void SceneBattle::Initialize()
{
	mBattleActorManager->Initialize();
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

	// �V���h�E�}�b�v
	mShadowMap.Activate(mLightDir);
	mTerrain->Render(mShadowMap.GetShader(), view, projection, mLightDir);
	mBattleActorManager->Render(mShadowMap.GetShader(), view, projection, mLightDir);
	mShadowMap.Deactivate();

	// �V�[���^�[�Q�b�g
	mSceneTarget.Activate();
	mSkybox->Render(view, projection);
	mTerrain->Render(view, projection, mLightDir);
	mBattleActorManager->Render(view, projection, mLightDir);
	Singleton<EffectManager>().GetInstance().Render(view, projection);
	mSceneTarget.Deactivate();

	mSceneTarget.Render(mPostEffect.get());
}

void SceneBattle::Release()
{
	Singleton<CameraManager>().GetInstance().Pop();
	BattleState::DestroyInst();
}
