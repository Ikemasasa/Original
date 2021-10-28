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

	// �t���O���Z�b�g
	GameManager::bossSubdueFlag = false;

	mIsLoadFinished = true;


	// BGM �Đ��J�n
	Audio::MusicPlay((int)Music::FIELD_REMAINS);
}

void SceneField::Update()
{
	if (mTransBattleScene)
	{
		static float timer = 0.0f;
		if (timer >= TRANS_BATTLE_SCENE_TIME)
		{
			// �V�[���ڍs
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

		// �G�Ɠ����������`�F�b�N
		if (mCharaManager->IsHitEnemy())
		{
			mTransBattleScene = true;
		}

		// �L�[�K�C�h
		KeyGuide::Instance().Add(KeyGuide::Y, L"���j���[");
	}
}

void SceneField::Render()
{
	Matrix view = Singleton<CameraManager>().GetInstance().GetView();
	Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();
	Vector4 lightDir = mLight.GetLightDir();

	// �V���h�E�}�b�v�ɏ�������
	const Shader* shader = mShadowMap->GetShader();
	mShadowMap->Activate(lightDir, mLight.GetLightPos());
	mTerrain->Render(shader, view, proj, lightDir);
	mCharaManager->Render(shader, view, proj, lightDir);
	mShadowMap->Deactivate();

	// GBuffer�ɏ�������
	shader = mDeferredRenderer->GetGBufferShader();
	mDeferredRenderer->ActivateGBuffer();
	mSkybox->Render(view, proj);
	mTerrain->Render(shader, view, proj, lightDir);
	mCharaManager->Render(shader, view, proj, lightDir);
	mDeferredRenderer->DeactivateGBuffer();

	// �e�N�X�`���Z�b�g(GBuffer, ��, �e
	mDeferredRenderer->SetGBufferTexture();
	mSkybox->SetEnvTexture(Define::ENVIRONMENT_TEXTURE_SLOT);
	mShadowMap->SetTexture(Define::SHADOWMAP_TEXTURE_SLOT);
	
	// �f�B�t�@�[�h�̃p�����[�^�p��(��X�O��������͂�����
	SetDeferredParam();

	// �f�B�t�@�[�h�����_�����O
	//// �V�[���^�[�Q�b�g�ɏ�������
	mPostEffectTarget->Activate();
	mDeferredRenderer->Render();
	// �u���[���쐬�A�K�p
	mBloom->Execute(mPostEffectTarget.get());
	mPostEffectTarget->Deactivate();

	// �V�[���^�[�Q�b�g�ɏ�������
	mSceneTarget->Activate();
	mPostEffectTarget->Render(mPostEffect.get());
	mCharaManager->RenderUI();
	mSceneTarget->Deactivate();

	// �o�b�N�o�b�t�@�ɏ�������
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
	// �f�B�t�@�[�h�̃��C�g�p��(��X�O��������͂�����
	//// ���C�g�N���A
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

		// ���W�A�����W�Z�o
		const float ADJUST_DIV = 1.2f;
		const AABB aabb = enm->GetLocalAABB();
		float range = (aabb.max.y - aabb.min.y) / ADJUST_DIV;
		pointLight.pos = Vector4(enm->GetPos(), range);
		pointLight.pos.y += aabb.max.y / ADJUST_DIV;
		pointLights.push_back(pointLight);
	}
	mDeferredRenderer->SetPointLight(pointLights);

	// �萔�o�b�t�@�X�V
	Vector3 eyePos = Singleton<CameraManager>().GetInstance().GetPos();
	mDeferredRenderer->SetCBPerFrame(eyePos);
}