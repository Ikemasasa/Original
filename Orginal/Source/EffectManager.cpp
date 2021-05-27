#include "EffectManager.h"

#include "lib/Framework.h"

#include "CameraBase.h"

EffectManager::EffectManager()
{
	// �G�t�F�N�g�̂̃����_���[�̍쐬
	ID3D11Device* device = FRAMEWORK.GetDevice();
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();
	mRenderer = EffekseerRendererDX11::Renderer::Create(device, context, SQUARE_MAX);

	// �}�l�[�W���[
	mManager = Effekseer::Manager::Create(SQUARE_MAX);

	// �`�惂�W���[���̐ݒ�
	mManager->SetSpriteRenderer(mRenderer->CreateSpriteRenderer());
	mManager->SetRibbonRenderer(mRenderer->CreateRibbonRenderer());
	mManager->SetRingRenderer(mRenderer->CreateRingRenderer());
	mManager->SetTrackRenderer(mRenderer->CreateTrackRenderer());
	mManager->SetModelRenderer(mRenderer->CreateModelRenderer());

	// �e�N�X�`���A���f���A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	mManager->SetTextureLoader(mRenderer->CreateTextureLoader());
	mManager->SetModelLoader(mRenderer->CreateModelLoader());
	mManager->SetMaterialLoader(mRenderer->CreateMaterialLoader());

	// ���W�n
	mManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);


	// �����o������
	memset(mInstHandles, -1, sizeof(Effekseer::Handle));
}

EffectManager::~EffectManager()
{
	for (auto& ef : mEffects)
	{
		if (ef) ef->Release();
		ef = nullptr;
	}
	mRenderer->Destroy();
	mManager->Destroy();
}


void EffectManager::Create(const EFK_CHAR* efkPath, const int& slot)
{
	if (mEffects[slot]) return;

	Effekseer::Effect* effect = Effekseer::Effect::Create(mManager, efkPath);
	mEffects[slot] = effect;
	mEffectNum++;
}

void EffectManager::Update()
{
	mManager->Update();

	// �C���X�^���X�`�F�b�N
	for (int i = 0; i < INST_MAX; ++i)
	{
		if (mInstHandles[i] == -1) continue;
		
		if (!mManager->Exists(mInstHandles[i]))
		{
			mInstHandles[i] = -1;
			mInstNum--;
		}
	}
}

void EffectManager::Render(const DirectX::XMFLOAT4X4& viewMatrix)
{
	Effekseer::Matrix44 view;
	memcpy(&view, &viewMatrix, sizeof(Effekseer::Matrix44));
	mRenderer->SetCameraMatrix(view);

	mRenderer->BeginRendering();
	mManager->Draw();
	mRenderer->EndRendering();
}

void EffectManager::SetProjectionMatrix(const DirectX::XMFLOAT4X4* projMatrix)
{
	Effekseer::Matrix44 proj;
	memcpy(&proj, projMatrix, sizeof(Effekseer::Matrix44));

	mRenderer->SetProjectionMatrix(proj);
}

int EffectManager::Play(const int& slot, const Vector3& pos, int startFrame, float scale, float speed)
{
	if (!mEffects[slot]) return -1;
	Effekseer::Handle h = mManager->Play(mEffects[slot], Effekseer::Vector3D(pos.x, pos.y, pos.z), startFrame);
	mManager->SetSpeed(h, speed);
	mManager->SetScale(h, scale, scale, scale);

	for (int i = 0; i < INST_MAX; ++i)
	{
		if (mInstHandles[i] != -1) continue;
		
		mInstHandles[i] = h;
		break;
	}

	return h;
}

