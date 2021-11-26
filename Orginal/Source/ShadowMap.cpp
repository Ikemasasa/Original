#include "ShadowMap.h"

#include "lib/Framework.h"

ShadowMap::ShadowMap()
{
	mShader = std::make_unique<Shader>();
	mShader->Load(L"Shaders/ShadowMap.fx", "VSMain", "PSMain");
}

ShadowMap::~ShadowMap()
{
	mShader->UnLoad();
}

void ShadowMap::Initialize() 
{
	ID3D11Device* device = FRAMEWORK.GetDevice();

	// 定数バッファ
	mConstBuffer.Create(sizeof(CBShadow));
	
	// バッファ作成
	mShadowMap.Initialize(SHADOWMAP_X, SHADOWMAP_Y, DXGI_FORMAT_R16G16_FLOAT);
	mVarianceShadowMap.Initialize(SHADOWMAP_X, SHADOWMAP_Y, DXGI_FORMAT_R16G16_FLOAT);

	mGaussianBlur.Initialize(Vector2(SHADOWMAP_X, SHADOWMAP_Y), DXGI_FORMAT_R16G16_FLOAT, BLUR_STRENGTH);

	mShadowProj.Ortho(200, 200, 0.1f, 1000.0f);
	mTarget = Vector3::ZERO;
}

void ShadowMap::Activate(const Vector3& target, const Vector3& dir, float maxDistance)
{
	// ターゲット設定
	//mTarget = target;

	// プロジェクション行列設定
	mShadowProj.Ortho(maxDistance, maxDistance, 0.1f, 1000.0f);

	// ライト座標算出
	Vector3 lightPos = -dir * maxDistance;


	// ビュー行列算出
	Matrix view;
	view.LookAtLH(lightPos, mTarget, Vector3(0, 1, 0));

	// 定数バッファを更新
	// 光から見た平行投影の距離が描画される
	CBShadow cb;
	cb.shadowVP = view * mShadowProj;
	mConstBuffer.Update(&cb);
	mConstBuffer.Set(5);

	// レンダーターゲット有効化
	mShadowMap.Activate();
}

void ShadowMap::Deactivate()
{
	// シャドウマップをもとにVSMを作る
	
	// VSMにシャドウマップにブラーをかけたものをレンダーする
	mShadowMap.Deactivate();
	mShadowMap.SetTexture(0);
	mGaussianBlur.Blur(&mShadowMap);

	mVarianceShadowMap.Activate();
	mGaussianBlur.Render();
	mVarianceShadowMap.Deactivate();
}

void ShadowMap::SetTexture(UINT textureSlot)
{
	mVarianceShadowMap.SetTexture(textureSlot);
}

