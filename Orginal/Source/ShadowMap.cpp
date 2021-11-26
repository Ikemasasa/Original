#include "ShadowMap.h"

#include "lib/Framework.h"

ShadowMap::ShadowMap()
{
	mShader = std::make_unique<Shader>();
	mShader->Load(L"Shaders/ShadowMap.fx", "VSMain", "PSMain");
}

ShadowMap::~ShadowMap()
{
	mShader->UnLoad();
}

void ShadowMap::Initialize() 
{
	ID3D11Device* device = FRAMEWORK.GetDevice();

	// �萔�o�b�t�@
	mConstBuffer.Create(sizeof(CBShadow));
	
	// �o�b�t�@�쐬
	mShadowMap.Initialize(SHADOWMAP_X, SHADOWMAP_Y, DXGI_FORMAT_R16G16_FLOAT);
	mVarianceShadowMap.Initialize(SHADOWMAP_X, SHADOWMAP_Y, DXGI_FORMAT_R16G16_FLOAT);

	mGaussianBlur.Initialize(Vector2(SHADOWMAP_X, SHADOWMAP_Y), DXGI_FORMAT_R16G16_FLOAT, BLUR_STRENGTH);

	mShadowProj.Ortho(200, 200, 0.1f, 1000.0f);
	mTarget = Vector3::ZERO;
}

void ShadowMap::Activate(const Vector3& target, const Vector3& dir, float maxDistance)
{
	// �^�[�Q�b�g�ݒ�
	//mTarget = target;

	// �v���W�F�N�V�����s��ݒ�
	mShadowProj.Ortho(maxDistance, maxDistance, 0.1f, 1000.0f);

	// ���C�g���W�Z�o
	Vector3 lightPos = -dir * maxDistance;


	// �r���[�s��Z�o
	Matrix view;
	view.LookAtLH(lightPos, mTarget, Vector3(0, 1, 0));

	// �萔�o�b�t�@���X�V
	// �����猩�����s���e�̋������`�悳���
	CBShadow cb;
	cb.shadowVP = view * mShadowProj;
	mConstBuffer.Update(&cb);
	mConstBuffer.Set(5);

	// �����_�[�^�[�Q�b�g�L����
	mShadowMap.Activate();
}

void ShadowMap::Deactivate()
{
	// �V���h�E�}�b�v�����Ƃ�VSM�����
	
	// VSM�ɃV���h�E�}�b�v�Ƀu���[�����������̂������_�[����
	mShadowMap.Deactivate();
	mShadowMap.SetTexture(0);
	mGaussianBlur.Blur(&mShadowMap);

	mVarianceShadowMap.Activate();
	mGaussianBlur.Render();
	mVarianceShadowMap.Deactivate();
}

void ShadowMap::SetTexture(UINT textureSlot)
{
	mVarianceShadowMap.SetTexture(textureSlot);
}

