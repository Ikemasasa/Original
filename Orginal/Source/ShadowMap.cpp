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
	//mShadowMap.CreateDSV(SHADOWMAP_X, SHADOWMAP_Y, DXGI_FORMAT_R24G8_TYPELESS);

	mGaussianBlur.Initialize(Vector2(SHADOWMAP_X, SHADOWMAP_Y), BLUR_STRENGTH);

	mTarget = Vector3::Zero();
}

void ShadowMap::Activate(const DirectX::XMFLOAT4& lightDir, int textureSlot)
{
	// �����_�[�^�[�Q�b�g�L����
	mShadowMap.Activate(textureSlot);


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
		DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicLH(150, 150, 0.1f, 600.0f);

		Vector3 lightPos(30.0f, 30.0f, 30.0f); // �K��
		lightPos.x *= -lightDir.x;
		//lightPos.y *= -lightDir.y;
		lightPos.z *= -lightDir.z;
		DirectX::XMVECTOR pos = DirectX::XMVectorSet(lightPos.x, lightPos.y, lightPos.z, 1.0f);
		DirectX::XMVECTOR target = DirectX::XMVectorSet(mTarget.x, mTarget.y, mTarget.z, 1.0f);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f,	0.0f, 0.0f);
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);

		// �����猩�����s���e�̋������`�悳���
		CBShadow cb;
		DirectX::XMStoreFloat4x4(&cb.shadowVP, view * proj);
		mConstBuffer.Update(&cb);

		// �Z�b�g
		mConstBuffer.Set(1);
	}
}

void ShadowMap::Deactivate(int textureSlot)
{
	// �V���h�E�}�b�v�����Ƃ�VSM�����
	
	// VSM�ɃV���h�E�}�b�v�Ƀu���[�����������̂������_�[����
	mShadowMap.Deactivate();

	mVarianceShadowMap.Activate();
	mGaussianBlur.Blur(&mShadowMap);
	mVarianceShadowMap.Deactivate(textureSlot);
}