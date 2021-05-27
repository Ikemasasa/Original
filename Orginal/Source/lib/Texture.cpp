#include "Texture.h"

#include "Framework.h"
#include "ResourceManager.h"
#include "Renderer2D.h"

Texture::Texture(const wchar_t* filename)
{
	ResourceManager::LoadShaderResource(FRAMEWORK.GetDevice(), filename, &mSRV, &mDesc);
}

Texture::~Texture()
{
	ResourceManager::ReleaseShaderResourceView(mSRV);
}

bool Texture::Load(const wchar_t* filename)
{
	if (mSRV) return false;

	HRESULT hr = ResourceManager::LoadShaderResource(FRAMEWORK.GetDevice(), filename, &mSRV, &mDesc);
	if(FAILED(hr)) return false;

	return true;
}

void Texture::UnLoad()
{
	ResourceManager::ReleaseShaderResourceView(mSRV);
}

void Texture::Set(u_int slot)
{
	if (!mSRV) return;

	FRAMEWORK.GetContext()->PSSetShaderResources(slot, 1, &mSRV);
}

void Texture::Render(const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center, const float angle, const Vector4 color)
{
	Renderer2D::GetInstance().Render(mSRV, pos, scale, texPos, size, center, angle, color);
}
