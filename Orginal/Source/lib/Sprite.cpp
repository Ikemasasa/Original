#include "Sprite.h"

#include "Framework.h"
#include "ResourceManager.h"
#include "Renderer2D.h"

Sprite::Sprite(const wchar_t* filename)
{
	ResourceManager::LoadShaderResource(FRAMEWORK.GetDevice(), filename, &mSRV, &mDesc);
}

Sprite::~Sprite()
{
	ResourceManager::ReleaseShaderResourceView(mSRV);
}

bool Sprite::Load(const wchar_t* filename)
{
	if (mSRV) return false;

	HRESULT hr = ResourceManager::LoadShaderResource(FRAMEWORK.GetDevice(), filename, &mSRV, &mDesc);
	if(FAILED(hr)) return false;

	return true;
}

void Sprite::UnLoad()
{
	ResourceManager::ReleaseShaderResourceView(mSRV);
}

void Sprite::Set(u_int slot)
{
	if (!mSRV) return;

	FRAMEWORK.GetContext()->PSSetShaderResources(slot, 1, &mSRV);
}

void Sprite::Render(const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center, const float angle, const Vector4 color)
{
	Renderer2D::GetInstance().Render(mSRV, pos, scale, texPos, size, center, angle, color);
}
