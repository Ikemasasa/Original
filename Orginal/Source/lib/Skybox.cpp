#include "Skybox.h"

#include "Framework.h"
#include "ResourceManager.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Sprite.h"


void Skybox::Initialize(const wchar_t* filename)
{
	mEyePos = Vector3::ZERO;

	// シェーダ作成
	mShader = std::make_unique<Shader>();
	mShader->Load2D(L"Shaders/SkyBox.fx", "VSMain", "PSMain");

	// 定数バッファ
	mConstBuffer.Create(sizeof(CbSky));

	// テクスチャ読み込み
	mTexture = std::make_unique<Sprite>(filename);
}

void Skybox::Release()
{
	if(mShader) mShader->UnLoad();
	if(mTexture) mTexture->UnLoad();
}

void Skybox::SetEyePos(Vector3 eye)
{
	mEyePos = eye;
}

void Skybox::Render(const Matrix& view, const Matrix& proj)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// 入力としてリソースが登録されていたら解除する
	if (mTextureSlot != -1)
	{
		ID3D11ShaderResourceView* dummy = nullptr;
		FRAMEWORK.GetContext()->PSSetShaderResources(mTextureSlot, 1, &dummy);
		mTextureSlot = -1; // リセット
	}

	// CB更新
	CbSky cb;
	cb.cameraPos.x = mEyePos.x;
	cb.cameraPos.y = mEyePos.y;
	cb.cameraPos.z = mEyePos.z;
	cb.cameraPos.w = 1.0f;
	cb.invView = view;
	cb.invView.Inverse();
	cb.invProj = proj;
	cb.invProj.Inverse();
	mConstBuffer.Update(&cb);
	mConstBuffer.Set(0);

	{
		// 描画
		Vector2 pos(0.0f, 0.0f);
		Vector2 scale (1.0f, 1.0f);
		Vector2 texPos(0.0f, 0.0f);
		Vector2 size(mTexture->GetSize().x, mTexture->GetSize().y);
		Renderer2D::GetInstance().Render(mTexture->GetSRV(), mShader.get(), pos, scale, texPos, size);
	}
}

void Skybox::SetEnvTexture(UINT slot)
{
	mTextureSlot = slot;
	ID3D11ShaderResourceView* srv = mTexture->GetSRV();
	FRAMEWORK.GetContext()->PSSetShaderResources(slot, 1, &srv);
}
