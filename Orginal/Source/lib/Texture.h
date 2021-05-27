#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "Vector.h"

class Texture
{
	// リソースマネージャーを通すからSRVは普通のポインタ
	ID3D11ShaderResourceView* mSRV						    = nullptr;
	D3D11_TEXTURE2D_DESC mDesc = {};

public:
	Texture() = default;
	Texture(const wchar_t* filename);
	~Texture();

	bool Load(const wchar_t* filename);
	void UnLoad();
	void Set(u_int slot);

	void Render(const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center = Vector2::Zero(), const float angle = 0.0f, const Vector4 color = Vector4::One());

	ID3D11ShaderResourceView* GetSRV() { return mSRV; }

	Vector2 GetSize() const { return Vector2(static_cast<float>(mDesc.Width), static_cast<float>(mDesc.Height)); }
};
