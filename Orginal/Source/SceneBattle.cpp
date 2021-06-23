#include "SceneBattle.h"

#include "lib/Input.h"
#include "lib/Skybox.h"

#include "BattleActorManager.h"
#include "BattleState.h"
#include "CameraBattle.h"
#include "CameraManager.h"
#include "EnemyBattle.h"
#include "Fade.h"
#include "FrontendBattle.h"
#include "PlayerBattle.h"
#include "SceneManager.h"
#include "Terrain.h"


SceneBattle::SceneBattle(PlayerManager* plm, Enemy* enemy)
{
	//FrontendBattle::CreateInst();
	BattleState::CreateInst();
	BattleState::GetInstance().SetState(BattleState::State::COMMAND_SELECT);

	mBattleActorManager = std::make_unique<BattleActorManager>(plm, enemy);
	mSkybox = std::make_unique<Skybox>(L"Data/Image/sky.png");


	mLightDir = { 0.0f, -1.0f, 1.0f, 0.0f };
}

SceneBattle::~SceneBattle()
{
	Singleton<CameraManager>().GetInstance().Pop();
	BattleState::DestroyInst();
	//FrontendBattle::DestroyInst();
}

void SceneBattle::Initialize()
{
	Singleton<CameraManager>().GetInstance().Push(std::make_shared<CameraBattle>());

	mBattleActorManager->Initialize();
}

void SceneBattle::Update()
{
	mBattleActorManager->Update();

	Singleton<CameraManager>().GetInstance().Update(mBattleActorManager->GetMoveActor());

	mSkybox->SetEyePos(Singleton<CameraManager>().GetInstance().GetPos());
}

void SceneBattle::Render()
{
	DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
	DirectX::XMFLOAT4X4 projection = Singleton<CameraManager>().GetInstance().GetProj();

	mSkybox->Render(view, projection);
	mBattleActorManager->Render(view, projection, mLightDir);
}

void SceneBattle::Release()
{

}
