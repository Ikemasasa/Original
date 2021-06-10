#include "Skybox.h"

#include "Framework.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Sprite.h"
#include "Window.h"

Skybox::Skybox(const wchar_t* filename)
{
	mConstBuffer = nullptr;
	mEyePos = Vector3::Zero();
	mShader = std::make_unique<Shader>();
	Initialize();

	mTexture = std::make_unique<Sprite>(filename);
}

Skybox::~Skybox()
{
	mShader->UnLoad();
	mTexture->UnLoad();
}

bool Skybox::Initialize()
{
	ID3D11Device* device = FRAMEWORK.GetDevice();

	bool check = false;
	check = CreateShaders();
	if (!check) return false;

	check = CreateConstantBuffer();
	if (!check) return false;

	return true;
}

bool Skybox::CreateShaders()
{
	// ���_�V�F�[�_ / �s�N�Z���V�F�[�_�@�̍쐬
	bool check = false;
	check = mShader->Load(L"Shaders/SkyBox.fx", "VSMain", "PSMain");
	if (!check) return false;

	return true;
}

bool Skybox::CreateConstantBuffer()
{
	ID3D11Device* device = FRAMEWORK.GetDevice();
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bd, NULL, mConstBuffer.GetAddressOf());
	if (FAILED(hr)) return false;

	return true;
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

	ConstantBuffer cb;
	cb.cameraPos.x = mEyePos.x;
	cb.cameraPos.y = mEyePos.y;
	cb.cameraPos.z = mEyePos.z;
	cb.cameraPos.w = 1.0f;
	DirectX::XMStoreFloat4x4(&cb.invView, DirectX::XMMatrixInverse(nullptr, matView));
	DirectX::XMStoreFloat4x4(&cb.invProj, DirectX::XMMatrixInverse(nullptr, matProj));
	context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, &cb, 0, 0);
	context->VSSetConstantBuffers(2, 1, mConstBuffer.GetAddressOf());
	context->PSSetConstantBuffers(2, 1, mConstBuffer.GetAddressOf());

	{
		// �`��
		Vector2 pos(0.0f, 0.0f);
		Vector2 scale (1.0f, 1.0f);
		Vector2 texPos(0.0f, 0.0f);
		Vector2 size(mTexture->GetSize().x, mTexture->GetSize().y);
		Renderer2D::GetInstance().Render(mTexture->GetSRV(), mShader.get(), pos, scale, texPos, size);
	}
}
