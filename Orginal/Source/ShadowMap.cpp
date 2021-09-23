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

	mTarget = Vector3::ZERO;
}

void ShadowMap::Activate(const Vector4& lightDir)
{
	// �����_�[�^�[�Q�b�g�L����
	mShadowMap.Activate();


	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp = {
		0,0,
		SHADOWMAP_X,
		SHADOWMAP_Y,
		0,1 };
	context->RSSetViewports(1, &vp);

	// �萔�o�b�t�@
	{
		DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicLH(200, 200, 0.1f, 1000.0f);

		Vector3 lightPos(60.0f, 60.0f, 60.0f); // �K��
		lightPos.x *= -lightDir.x;
		//lightPos.y *= -lightDir.y;
		lightPos.z *= -lightDir.z;
		DirectX::XMVECTOR pos = DirectX::XMVectorSet(lightPos.x, lightPos.y, lightPos.z, 1.0f);
		DirectX::XMVECTOR target = DirectX::XMVectorSet(mTarget.x, mTarget.z, mTarget.z, 1.0f);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f,	0.0f, 0.0f);
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);

		// �����猩�����s���e�̋������`�悳���
		CBShadow cb;
		DirectX::XMStoreFloat4x4(&cb.shadowVP, view * proj);
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

