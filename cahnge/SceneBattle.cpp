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
	// SceneBase�̊e�평����
	mShadowMap.Initialize();
	mSceneTarget.Initialize();
	CreatePostEffectShader();

	// SceneBattle�̊e�평����
	BattleState::CreateInst();
	BattleState::GetInstance().SetState(BattleState::State::BEGIN);

	// plm, enemy�ۑ�
	mPlayerManager = plm;
	mHitEnemy = enemy;
	
	mTurnManager			= std::make_unique<TurnManager>();
	mBattleCharacterManager = std::make_unique<BattleCharacterManager>(plm, enemy);
	mSkybox					= std::make_unique<Skybox>();
	mTerrain				= std::make_shared<Terrain>(DataBase::TERRAIN_ID_START);

	// BGM����
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

		// �t�B�[���h�ɖ߂�
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			Fade::GetInstance().SetSceneImage(Fade::SPEED_SLOW);
			SceneManager::GetInstance().PopCurrentScene();
		}
	}

	mTurnManager->Update(mBattleCharacterManager.get());
	mBattleCharacterManager->Update(mTurnManager.get());

	// 1�^�[�����I��������
	if (mTurnManager->IsTurnFinished())
	{
		// ����ł�A�N�^�[���`�F�b�N
		mBattleCharacterManager->OrganizeCharacter();

		// �o�g���I�����`�F�b�N
		mResult = mBattleCharacterManager->CheckBattleFinish();
		if (mResult != NONE)
		{
			// State��Result�ɂ���
			BattleState::GetInstance().SetState(BattleState::State::RESULT);

			switch (mResult)
			{
			case PLAYER_WIN:
				// �퓬��̃X�e�[�^�X���X�V
				for (int i = 0; i < mPlayerManager->GetNum(); ++i)
				{
					BattleCharacter* pl = mBattleCharacterManager->GetChara(i);
					pl->GetStatus()->ResetBuff(); // �o�t����
					Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(pl->GetCharaID(), *pl->GetStatus());
				}

				// �h���b�v�A�C�e�����C���x���g���ɉ�����
				for (auto& dropID : mBattleCharacterManager->GetDropItemIDs())
				{
					mPlayerManager->GetEquipmentInventory()->Push(dropID);
				}

				// BGM�����U���g�̂�ɂ���
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

	// �V���h�E�}�b�v
	mShadowMap.Activate(lightDir, SHADOWMAP_TEXTURE_SLOT);
	mTerrain->Render(mShadowMap.GetShader(), view, projection, lightDir);
	mBattleCharacterManager->Render(mShadowMap.GetShader(), view, projection, lightDir);
	mShadowMap.Deactivate(SHADOWMAP_TEXTURE_SLOT);

	// �V�[���^�[�Q�b�g
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

	// BGM�X�g�b�v�A�t�B�[���hBGM�Đ�
	AUDIO.MusicStop((int)result);
	AUDIO.MusicStop((int)music);
	AUDIO.MusicPlay((int)Music::FIELD_REMAINS);
}
