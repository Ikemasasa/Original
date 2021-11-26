#pragma once
#include <DirectXmath.h>
#include <memory>

#include "ConstantBuffer.h"
#include "Matrix.h"
#include "Vector.h"

class Sprite;
class Shader;

// スカイボックスを描画するクラス
class Skybox
{
	// 変数
	struct CbSky
	{
		Vector4 cameraPos;
		Matrix invView;
		Matrix invProj;
	};
	ConstantBuffer mConstBuffer;	// 定数バッファ

	std::unique_ptr<Sprite> mTexture; // スカイボックスの画像
	std::unique_ptr<Shader> mShader;  // スカイボックスシェーダ
	Vector3 mEyePos = {};			  // カメラ座標
	UINT mTextureSlot = -1;			  // パイプラインにセットするスロット

public:// 関数

	// コンストラクタ
	Skybox() = default;

	// デストラクタ
	~Skybox() = default;
	
	// 初期化
	void Initialize(const wchar_t* filename);
	
	// 解放
	void Release();

	// 描画
	void Render(const Matrix& view, const Matrix& proj);

	// カメラ座標設定
	void SetEyePos(Vector3 eye);

	// 画像をパイプラインに設定
	void SetEnvTexture(UINT slot);
};