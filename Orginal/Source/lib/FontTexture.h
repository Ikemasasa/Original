#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "Vector.h"

class FontTexture
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			 mTex2D   = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV     = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>			 mOutlineTex = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mOutlineSRV = nullptr;
	Vector2 mTextureSize = {};

public:
	FontTexture() = default;
	~FontTexture() = default;
	bool Create(const wchar_t chara, const WCHAR* fontname, int fontSize, int fontWeight, GLYPHMETRICS* outGM, TEXTMETRIC* outTm);

	Vector2 GetSize() const { return mTextureSize; }
	ID3D11ShaderResourceView* GetSRV() const { return mSRV.Get(); }
};