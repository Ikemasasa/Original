#include "SceneBattle.h"

#include "lib/Audio.h"
#include "lib/Skybox.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraBattle.h"
#include "CameraManager.h"
#include "DataBase.h"
#include "EffectManager.h"
#include "Light.h"
#include "SceneManager.h"
#include "Terrain.h"


SceneBattle::SceneBattle(PlayerManager* plm, Enemy* enemy)
{
	// SceneBase�̊e�평����
	mShadowMap.Initialize();
	mSceneTarget.Initialize();
	CreatePostEffectShader();

	// SceneBattle�̊e�평����
	BattleState::CreateInst();
	BattleState::GetInstance().SetState(BattleState::State::COMMAND_SELECT);

	mBattleCharacterManager = std::make_unique<BattleCharacterManager>(plm, enemy);
	mSkybox = std::make_unique<Skybox>();
	mTerrain = std::make_shared<Terrain>(DataBase::TERRAIN_ID_START);
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


	AUDIO.MusicPlay((int)Music::BATTLE);
}

void SceneBattle::Update()
{
	mBattleCharacterManager->Update();

	Singleton<CameraManager>().GetInstance().Update(mBattleCharacterManager->GetMoveChara());
	mSkybox->SetEyePos(Singleton<CameraManager>().GetInstance().GetPos());
	Singleton<EffectManager>().GetInstance().Update();
}

void SceneBattle::Render()
{
	DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
	DirectX::XMFLOAT4X4 projection = Singleton<CameraManager>().GetInstance().GetProj();
	DirectX::XMFLOAT4 lightDir = mLight.GetLightDir();

	// �V���h�E�}�b�v
	mShadowMap.Activate(lightDir, SHADOWMAP_TEXTURE_SLOT);
	mTerrain->Render(mShadowMap.GetShader(), view, projection, lightDir);
	mBattleCharacterManager->Render(mShadowMap.GetShader(), view, projection, lightDir);
	mShadowMap.Deactivate(SHADOWMAP_TEXTURE_SLOT);

	// �V�[���^�[�Q�b�g
	mSceneTarget.Activate();
	mSkybox->Render(view, projection);
	mTerrain->Render(view, projection, lightDir);
	mBattleCharacterManager->Render(view, projection, lightDir);
	Singleton<EffectManager>().GetInstance().Render(view, projection);
	mSceneTarget.Deactivate();

	mSceneTarget.Render(mPostEffect.get());
}

void SceneBattle::Release()
{
	Singleton<CameraManager>().GetInstance().Pop();
	BattleState::DestroyInst();

	// ���U���gBGM�X�g�b�v�A�t�B�[���hBGM�Đ�
	AUDIO.MusicStop((int)Music::RESULT);
	AUDIO.MusicPlay((int)Music::FIELD_REMAINS);
}
