#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "Vector.h"

// ����A�ł���Ȃ當�����1���̃e�N�X�`���ɏ������߂�悤�ɂ�����
// �ꕶ�����̏������ł��Ȃ��Ȃ邯�ǁA���邩�ǂ����͉��^�I�Ȃ̂ŁB�B�B

// ����Create�� gm, tm ������𕪂���Ƃ킩��₷���Ȃ肻��

class FontTexture
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			 mTex2D   = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV     = nullptr;
	Vector2 mTextureSize;

public:
	FontTexture() = default;
	~FontTexture() = default;
	bool Create(const wchar_t chara, const WCHAR* fontname, int fontSize, int fontWeight, GLYPHMETRICS* outGM, TEXTMETRIC* outTm);

	Vector2 GetSize() const { return mTextureSize; }
	ID3D11ShaderResourceView* GetSRV() const { return mSRV.Get(); }
};