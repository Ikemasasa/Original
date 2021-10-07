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

void ShadowMap::Activate(const Vector4& lightDir, const Vector3& lightPos)
{
	// �����_�[�^�[�Q�b�g�L����
	mShadowMap.Activate();


	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// �萔�o�b�t�@
	{
		Matrix view;
		view.LookAtLH(lightPos, mTarget, Vector3(0, 1, 0));

		// �����猩�����s���e�̋������`�悳���
		CBShadow cb;
		cb.shadowVP = view * mShadowProj;
		mConstBuffer.Update(&cb);

		// �Z�b�g
		mConstBuffer.Set(5);
	}
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

