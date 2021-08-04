#include "Skybox.h"

#include "Framework.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Sprite.h"

void Skybox::Initialize(const wchar_t* filename)
{
	mEyePos = Vector3::ZERO;

	// �V�F�[�_�쐬
	mShader = std::make_unique<Shader>();
	mShader->Load2D(L"Shaders/SkyBox.fx", "VSMain", "PSMain");

	// �萔�o�b�t�@
	mConstBuffer.Create(sizeof(CbSky));

	// �e�N�X�`���ǂݍ���
	mTexture = std::make_unique<Sprite>(filename);
}

void Skybox::Release()
{
	mShader->UnLoad();
	mTexture->UnLoad();
}

void Skybox::SetEyePos(Vector3 eye)
{
	mEyePos = eye;
}

void Skybox::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	DirectX::XMMATRIX matView = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX matProj =	DirectX::XMLoadFloat4x4(&proj);

	// CB�X�V
	CbSky cb;
	cb.cameraPos.x = mEyePos.x;
	cb.cameraPos.y = mEyePos.y;
	cb.cameraPos.z = mEyePos.z;
	cb.cameraPos.w = 1.0f;
	DirectX::XMStoreFloat4x4(&cb.invView, DirectX::XMMatrixInverse(nullptr, matView));
	DirectX::XMStoreFloat4x4(&cb.invProj, DirectX::XMMatrixInverse(nullptr, matProj));
	mConstBuffer.Update(&cb);
	mConstBuffer.Set(0);

	{
		// �`��
		Vector2 pos(0.0f, 0.0f);
		Vector2 scale (1.0f, 1.0f);
		Vector2 texPos(0.0f, 0.0f);
		Vector2 size(mTexture->GetSize().x, mTexture->GetSize().y);
		Renderer2D::GetInstance().Render(mTexture->GetSRV(), mShader.get(), pos, scale, texPos, size);
	}
}