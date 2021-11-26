#include "SceneTitle.h"

#include "lib/Input.h"

#include "CameraTitle.h"
#include "Define.h"
#include "Fade.h"
#include "KeyGuide.h"
#include "Music.h"
#include "PlayerManager.h"
#include "SceneField.h"
#include "SceneLoad.h"
#include "SceneManager.h"
#include "Sound.h"
#include "TransformData.h"

SceneTitle::SceneTitle()
{
	SceneBase::CreateBaseAll();

	mBG = std::make_unique<Sprite>(L"Data/Image/title_bg.png");
	mBoard = std::make_unique<Sprite>(L"Data/Image/Menu/str_board.png");
	mBoardSelect = std::make_unique<Sprite>(L"Data/Image/Menu/str_select.png");

	mCamera = std::make_unique<CameraTitle>();
	mPlayer = std::make_unique<Character>(PlayerManager::ERIKA, Character::PLAYER);
	mTerrain = std::make_unique<Terrain>(DataBase::TERRAIN_ID_START);
	mSkybox = std::make_unique<Skybox>();
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Initialize()
{
	SceneBase::InitializeBaseAll();

	// �v���C���[�̃g�����X�t�H�[��
	TransformData::Transform transform = TransformData::GetPLTransform(mPlayer->GetCharaID());
	mPlayer->SetPos(transform.pos);
	mPlayer->SetScale(transform.scale);
	mPlayer->SetAngle(transform.angle);
	mPlayer->SetCapsuleParam(transform.diameter / 2.0f);
	mPlayer->SetMotion(Character::IDLE);

	// ���߂�
	mCamera->Initialize(mPlayer.get()->GetTargetPos());

	// �X�J�C�{�b�N�X
	mSkybox->Initialize(L"Data/Image/environment.hdr");

	// �t�H���g
	mFont.Initialize();
	mFontLogo.Initialize(FONT_LOGO_SIZE, FONT_LOGO_WEIGHT);

	// ���C�g�ݒ�
	{
		Vector3 lightDir(-1.0f, -0.8f, 1.0f);
		mLight.SetLightDir(lightDir);

		Vector4 lightColor(0.8f, 0.75f, 0.75f, 1.0f);
		mLight.SetLightColor(lightColor);

		mLight.CreateConstBuffer();
	}

	mIsPressAButton = false;
	mSelectIndex = 0;

	Music::Play(Music::TITLE);
}

void SceneTitle::Update()
{
	// ���S
	const wchar_t* logoStr = L"R P G";
	Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.20f);
	Vector2 center(0.5f, 0.0f);
	mFontLogo.RenderSet(logoStr, pos, center, Define::FONT_COLOR);

	// �I�����镶����
	if (!mIsPressAButton)
	{
		const wchar_t* pressAButton = L"PRESS A BUTTON";
		float y = Define::SCREEN_HEIGHT * 0.25f;
		Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.7f);
		Vector2 center(0.5f, 0.5f);
		Vector4 color(Define::FONT_COLOR);
		mSinAngle += Define::PI / 40.0f; // ���l�͓K��
		color.w = sinf(mSinAngle) + 1.0f * 0.5f;
		mFont.RenderSet(pressAButton, pos, center, color);

		if (Input::GetButtonTrigger(0, Input::A))
		{
			mIsPressAButton = true;
			Sound::Play(Sound::SELECT_LONG);
		}
	}
	else
	{
		// GAME_START(0) ~ EXIT(1)�ɗ}����
		int old = mSelectIndex;
		if (Input::GetButtonTrigger(0, Input::UP))   mSelectIndex = (mSelectIndex + MAX - 1) % MAX;
		if (Input::GetButtonTrigger(0, Input::DOWN)) mSelectIndex = (mSelectIndex + 1) % MAX;
		if (old != mSelectIndex) Sound::Play(Sound::CURSOR_MOVE);

		if (Input::GetButtonTrigger(0, Input::A))
		{
			Fade::GetInstance().Set(Fade::SPEED_SLOW);
			Sound::Play(Sound::SELECT);
		}
		if (Fade::GetInstance().IsFadeOutEnd())
		{
			switch (mSelectIndex)
			{
			case GAME_START: SceneManager::GetInstance().SetNextScene(std::make_unique<SceneLoad>(std::make_unique<SceneField>())); break;
			case EXIT: SceneManager::GetInstance().PopCurrentScene(); break;
			}
		}
	}

	mPlayer->UpdateWorld();

	// �J����
	mCamera->Update(mPlayer.get());

	mSkybox->SetEyePos(mCamera->GetPos());
}

void SceneTitle::Render()
{
	// mBG->Render(Vector2::ZERO, Vector2::ONE, Vector2::ZERO, mBG->GetSize());

	Matrix view = mCamera->GetViewMatrix();
	Matrix proj = mCamera->GetProjectionMatrix();
	Vector4 lightDir = Vector4(mLight.GetLightDir(), 1.0f);


	// �V���h�E�}�b�v�ɏ�������
	const Shader* shader = mShadowMap->GetShader();
	mShadowMap->Activate(mPlayer->GetTargetPos(), mLight.GetLightDir());
	mTerrain->Render(shader, view, proj, lightDir);
	mPlayer->Render(shader, view, proj, lightDir);
	mShadowMap->Deactivate();

	// GBuffer�ɏ�������
	shader = mDeferredRenderer->GetGBufferShader();
	mDeferredRenderer->ActivateGBuffer();
	mSkybox->Render(view, proj);
	mTerrain->Render(shader, view, proj, lightDir);
	mPlayer->Render(shader, view, proj, lightDir);
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
	RenderBoard();
	mFont.Render();
	mFontLogo.Render();
	mSceneTarget->Deactivate();

	// �o�b�N�o�b�t�@�ɏ�������
	mSceneTarget->Render(nullptr);
}

void SceneTitle::Release()
{
	mFont.Release();
}

void SceneTitle::RenderBoard()
{
	if (mIsPressAButton)
	{
		const wchar_t* SelectStr[MAX] =
		{
			L"GAME START",
			L"EXIT",
		};
		// �{�[�h�̕`��
		float d = Define::SCREEN_HEIGHT - Define::SCREEN_HEIGHT / 2.0f;
		const float OFFSET_Y = (d - mBoard->GetSize().y * MAX) / (MAX + 1);
		for (int i = 0; i < MAX; ++i)
		{
			Vector2 center(mBoard->GetSize().x / 2.0f, 0.0f);
			Vector2 pos(Define::SCREEN_WIDTH / 2.0f, Define::SCREEN_HEIGHT / 2.0f + OFFSET_Y * (i + 1));
			mBoard->Render(pos, Vector2::ONE, Vector2::ZERO, mBoard->GetSize(), center);
			if (mSelectIndex == i)
			{
				mBoardSelect->Render(pos, Vector2::ONE, Vector2::ZERO, mBoardSelect->GetSize(), center);
			}

			// �t�H���g�Z�b�g
			pos.y += STR_OFFSET;
			center.x = 0.5f;
			mFont.RenderSet(SelectStr[i], pos, center, Define::FONT_COLOR);
		}
	}
}

void SceneTitle::SetDeferredParam()
{
	//// ���C�g�N���A
	mDeferredRenderer->ClearLight();

	mDeferredRenderer->SetCBPerFrame(mCamera->GetPos());

	//// DirLight
	std::vector<DeferredRenderer::DirLight> dirLights;
	DeferredRenderer::DirLight dirLight;
	dirLight.dir = Vector4(mLight.GetLightDir(), 1.0f);
	dirLight.color = mLight.GetLightColor();
	dirLights.push_back(dirLight);
	mDeferredRenderer->SetDirLight(dirLights);

}
