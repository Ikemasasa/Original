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
#include "Music.h"
#include "Player.h"
#include "PlayerManager.h"
#include "SelectOptions.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "Terrain.h"
#include "TurnManager.h"

SceneBattle::SceneBattle(PlayerManager* plm, Enemy* enemy)
{
	// SceneBase�̊e�평����
	CreateBaseAll();

	// SceneBattle�̊e�평����
	BattleState::CreateInst();
	BattleState::GetInstance().SetState(BattleState::State::BEGIN);

	// plm, enemy�ۑ�
	mPlayerManager = plm;
	mHitEnemy = enemy;

	mSelectOptions = std::make_unique<SelectOptions>();
	mTurnManager = std::make_unique<TurnManager>();
	mBattleCharacterManager = std::make_unique<BattleCharacterManager>(plm, enemy);
	mSkybox = std::make_unique<Skybox>();
	mTerrain = std::make_shared<Terrain>(DataBase::TERRAIN_ID_START);

	// BGM����
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


	mSkybox->Initialize(L"Data/Image/environment.hdr");
	mTerrain->Initialize();
	mBattleCharacterManager->Initialize();
	mTurnManager->Initialize(mBattleCharacterManager->GetBCharacters());

	mSelectOptions->Initialize();


	// �J����������
	Singleton<CameraManager>().GetInstance().Push(std::make_unique<CameraBattle>());

	// targetY���Z�o�i�G��posY�̕��ς����߂�
	Vector3 target = {0,0,BattleCharacterManager::ENEMY_POS_Z};
	const std::vector<int> ids = mBattleCharacterManager->GetAliveObjIDs(Character::Type::ENEMY);
	for (const auto& id : ids)
	{
		target.y += mBattleCharacterManager->GetChara(id)->GetTargetPos().y;
	}
	target /= ids.size();
	Singleton<CameraManager>().GetInstance().Initialize(target, CameraBattle::BEGIN_START_DISTANCE);

	Music::Play(mBattleMusic);
}

void SceneBattle::Update()
{
	if (BattleState::GetInstance().CheckState(BattleState::State::RESULT))
	{
		if (mResult == PLAYER_LOSE)
		{
			mSelectOptions->AddTitle(L"���Ȃ����܂����H");
			mSelectOptions->AddOption(L"�G�ɓ����钼�O�ɖ߂�");
			mSelectOptions->AddOption(L"�^�C�g���ɖ߂�");
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
			// �t�B�[���h�ɖ߂�
			if (Input::GetButtonTrigger(0, Input::BUTTON::A))
			{
				Fade::GetInstance().SetSceneImage(Fade::SPEED_SLOW);
				SceneManager::GetInstance().PopCurrentScene();
			}
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
				// ���������G������
				mHitEnemy->SetExist(false);

				// �퓬��̃X�e�[�^�X�A�C���x���g�����X�V
				for (size_t i = 0; i < mPlayerManager->GetNum(); ++i)
				{
					BattleCharacter* pl = mBattleCharacterManager->GetChara(i);
					pl->GetStatus()->ResetBuff(); // �o�t����
					StatusData::SetPLStatus(pl->GetCharaID(), *pl->GetStatus());

					mPlayerManager->GetPlayer(i)->SetInventory(pl->GetInventory());
				}

				// �h���b�v�A�C�e�����C���x���g���ɉ�����
				for (auto& dropID : mBattleCharacterManager->GetDropItemIDs())
				{
					const ItemData::BaseData* base = ItemData::GetBaseData(dropID);
					
					switch (base->type)
					{
					case ItemData::EQUIPMENT: mPlayerManager->GetEquipmentInventory()->Push(dropID); break;
					case ItemData::FIELD_USE: mPlayerManager->GetPlayer(0)->GetInventory()->Add(dropID); break;
					}
					
				}

				// BGM�����U���g�̂�ɂ���
				Music::Play(mResultMusic);

				break;

			case PLAYER_LOSE:
				break;
			}
		}
	}

	Singleton<CameraManager>().GetInstance().Update(mBattleCharacterManager->GetMoveChara(), mBattleCharacterManager.get()); 
	mSkybox->SetEyePos(Singleton<CameraManager>().GetInstance().GetPos());
	Singleton<EffectManager>().GetInstance().Update();
}

void SceneBattle::Render()
{
	Matrix view = Singleton<CameraManager>().GetInstance().GetView();
	Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();
	Vector4 lightDir = Vector4(mLight.GetLightDir(), 1.0f);

	// �V���h�E�}�b�v
	const Shader* shader = mShadowMap->GetShader();
	mShadowMap->Activate(Vector3::ZERO, mLight.GetLightDir());
	mTerrain->Render(shader, view, proj, lightDir);
	mBattleCharacterManager->Render(shader, view, proj, lightDir);
	mShadowMap->Deactivate();

	// GBuffer�ɏ�������
	shader = mDeferredRenderer->GetGBufferShader();
	mDeferredRenderer->ActivateGBuffer();
	mSkybox->Render(view, proj);
	mTerrain->Render(shader, view, proj, lightDir);
	mBattleCharacterManager->Render(shader, view, proj, lightDir);
	mDeferredRenderer->DeactivateGBuffer();

	// �e�N�X�`���Z�b�g(Gbuffer, ��, �e
	mDeferredRenderer->SetGBufferTexture();
	mSkybox->SetEnvTexture(Define::ENVIRONMENT_TEXTURE_SLOT);
	mShadowMap->SetTexture(Define::SHADOWMAP_TEXTURE_SLOT);

	// �f�B�t�@�[�h�̃��C�g�p��(��X�O��������͂�����
	//// ���C�g�N���A
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

	// �f�B�t�@�[�h�����_�����O
	//// �|�X�g�G�t�F�N�g�^�[�Q�b�g�ɏ�������
	mPostEffectTarget->Activate();
	Singleton<EffectManager>().GetInstance().Render(view, proj);
	mDeferredRenderer->Render();
	// �u���[���쐬�A�K�p
	mBloom->Execute(mSceneTarget.get());
	mPostEffectTarget->Deactivate();

	// �V�[���^�[�Q�b�g�ɏ�������
	mSceneTarget->Activate();
	mPostEffectTarget->Render(mPostEffect.get());
	mTurnManager->Render();
	mBattleCharacterManager->RenderUI();
	mSelectOptions->Render(Vector2(OPTIONS_X, OPTIONS_Y));
	mSceneTarget->Deactivate();

	// �o�b�N�o�b�t�@�Ɍ��ʂ�`��
	mSceneTarget->Render(nullptr);
}

void SceneBattle::Release()
{
	Singleton<CameraManager>().GetInstance().Pop();
	BattleState::DestroyInst();

	// �t�B�[���hBGM�Đ�
	Music::Play(Music::FIELD_REMAINS);
}
