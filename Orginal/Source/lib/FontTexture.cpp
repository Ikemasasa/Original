#include "FontTexture.h"

#include <vector>

#include "Framework.h"

bool FontTexture::Create(const wchar_t chara, const WCHAR* fontname, int fontSize, int fontWeight, GLYPHMETRICS* outGM, TEXTMETRIC* outTm)
{
	ID3D11Device* device = FRAMEWORK.GetDevice();
	HRESULT hr = S_OK;

	// フォントの生成
	LOGFONT lf = { fontSize, 0, 0, 0, fontWeight, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, TEXT("\0") };
	wcscpy_s(lf.lfFaceName, LF_FACESIZE, fontname);
	HFONT hFont = CreateFontIndirect(&lf);

	// デバイスコンテキストのハンドル取得
	HDC hdc = GetDC(NULL);
	SelectObject(hdc, hFont);

	// TEXTMETRIC を取得
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	// テクスチャ作成
	{
		mTextureSize.x = static_cast<float>(tm.tmMaxCharWidth);
		mTextureSize.y = static_cast<float>(tm.tmHeight);

		// テクスチャ作成
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = static_cast<UINT>(mTextureSize.x);
		desc.Height = static_cast<UINT>(mTextureSize.y);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = device->CreateTexture2D(&desc, NULL, mTex2D.GetAddressOf());
		if (FAILED(hr)) return false;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		srvd.Format = desc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0;
		srvd.Texture2D.MipLevels = desc.MipLevels;
		hr = device->CreateShaderResourceView(mTex2D.Get(), &srvd, mSRV.GetAddressOf());
		if (FAILED(hr)) return false;

		//// アウトラインテクスチャ
		//ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		//desc.Width = static_cast<UINT>(mTextureSize.x + 2);
		//desc.Height = static_cast<UINT>(mTextureSize.y + 2);
		//desc.MipLevels = 1;
		//desc.ArraySize = 1;
		//desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//desc.SampleDesc.Count = 1;
		//desc.Usage = D3D11_USAGE_DYNAMIC;
		//desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//hr = device->CreateTexture2D(&desc, NULL, mOutlineTex.GetAddressOf());
		//if (FAILED(hr)) return false;

		//ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		//srvd.Format = desc.Format;
		//srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//srvd.Texture2D.MostDetailedMip = 0;
		//srvd.Texture2D.MipLevels = desc.MipLevels;
		//hr = device->CreateShaderResourceView(mOutlineTex.Get(), &srvd, mOutlineSRV.GetAddressOf());
		//if (FAILED(hr)) return false;

	}

	{
		ID3D11DeviceContext* context = FRAMEWORK.GetContext();

		BYTE* pMono = nullptr;

		D3D11_MAPPED_SUBRESOURCE map;
		context->Map(mTex2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		BYTE* pBits = (BYTE*)map.pData;
		memset(pBits, 0x00, static_cast<size_t>(mTextureSize.x * mTextureSize.y * 4));

		GLYPHMETRICS gm;
		CONST MAT2 mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
		DWORD size = GetGlyphOutline(hdc, (UINT)chara, GGO_GRAY4_BITMAP, &gm, 0, NULL, &mat);
		pMono = new BYTE[size];
		GetGlyphOutline(hdc, chara, GGO_GRAY4_BITMAP, &gm, size, pMono, &mat);

		// フォント情報の書き込み
		//std::vector<BYTE> alphas;
		int grad = 16;
		int fontWidth = (gm.gmBlackBoxX + 3) / 4 * 4;
		int fontHeight = gm.gmBlackBoxY;
		for (int y = 0; y < fontHeight; ++y)
		{
			for (int x = 0; x < fontWidth; ++x)
			{
				BYTE alpha = pMono[y * fontWidth + x] * 0xff / grad;
				UINT index = y * map.RowPitch + x * 4;
				pBits[index + 0] = static_cast<BYTE>(0xff);
				pBits[index + 1] = static_cast<BYTE>(0xff);
				pBits[index + 2] = static_cast<BYTE>(0xff);
				pBits[index + 3] = alpha;
				//alphas.emplace_back(alpha);
			}
		}
		delete[] pMono;
		context->Unmap(mTex2D.Get(), 0);

		//// アウトライン情報の書き込み
		//context->Map(mOutlineTex.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		//pBits = (BYTE*)map.pData;
		//memset(pBits, 0x00, static_cast<size_t>(mTextureSize.x + 2 * mTextureSize.y + 2 * 4));
		//for (int y = 0; y < fontHeight + 2; ++y)
		//{
		//	for (int x = 0; x < fontWidth + 2; ++x)
		//	{
		//		const char threshold = 0xff * 0.3f;

		//		int refNum = 0;
		//		int totalAlpha = 0;
		//		for (int i = 0; i < 9; ++i)
		//		{
		//			if (i == 4) continue;
		//			int xAdd = i % 3 - 1;
		//			int yAdd = i / 3 - 1;
		//			int xCheck = x + i % 3 - 1;
		//			int yCheck = y + i / 3 - 1;

		//			// 範囲外チェック
		//			if (xCheck < 0 || xCheck > fontWidth) continue;
		//			if (yCheck < 0 || yCheck > fontHeight) continue;

		//			int alphaIndex = yCheck * fontWidth + xCheck;
		//			totalAlpha += alphas[alphaIndex];
		//			++refNum;
		//		}
		//		BYTE alpha = static_cast<BYTE>(totalAlpha / refNum);

		//		UINT index = y * map.RowPitch + x * 4;
		//		pBits[index + 0] = static_cast<BYTE>(0xff);
		//		pBits[index + 1] = static_cast<BYTE>(0xff);
		//		pBits[index + 2] = static_cast<BYTE>(0xff);
		//		pBits[index + 3] = alpha;
		//	}
		//}
		//context->Unmap(mOutlineTex.Get(), 0);

		// outGm, tm代入
		*outGM = gm;
		*outTm = tm;
	}

	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	return true;
}