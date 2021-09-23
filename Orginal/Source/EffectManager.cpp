#include "EffectManager.h"

#include "lib/Framework.h"

#include "CameraBase.h"

EffectManager::EffectManager()
{
	// エフェクトののレンダラーの作成
	ID3D11Device* device = FRAMEWORK.GetDevice();
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();
	mRenderer = EffekseerRendererDX11::Renderer::Create(device, context, SQUARE_MAX);

	// マネージャー
	mManager = Effekseer::Manager::Create(SQUARE_MAX);

	// 描画モジュールの設定
	mManager->SetSpriteRenderer(mRenderer->CreateSpriteRenderer());
	mManager->SetRibbonRenderer(mRenderer->CreateRibbonRenderer());
	mManager->SetRingRenderer(mRenderer->CreateRingRenderer());
	mManager->SetTrackRenderer(mRenderer->CreateTrackRenderer());
	mManager->SetModelRenderer(mRenderer->CreateModelRenderer());

	// テクスチャ、モデル、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	mManager->SetTextureLoader(mRenderer->CreateTextureLoader());
	mManager->SetModelLoader(mRenderer->CreateModelLoader());
	mManager->SetMaterialLoader(mRenderer->CreateMaterialLoader());

	// 座標系
	mManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);


	// メンバ初期化
	memset(mInstHandles, -1, sizeof(Effekseer::Handle));
}

EffectManager::~EffectManager()
{
}


int EffectManager::Create(const EFK_CHAR* efkPath)
{
	if (mEffectNum >= EFFECT_MAX) return -1;

	int slot = mEffectNum;
	Effekseer::EffectRef effect = Effekseer::Effect::Create(mManager, efkPath);
	mEffects[slot] = effect;
	mEffectNum++;

	return slot;
}

void EffectManager::Create(const EFK_CHAR* efkPath, int slot)
{
	// 最大数を超えてる、スロットにすでに入ってる場合はreturn
	if (mEffectNum >= EFFECT_MAX) return;
	if (mEffects[slot].Get()) return;

	Effekseer::EffectRef effect = Effekseer::Effect::Create(mManager, efkPath);
	mEffects[slot] = effect;
	mEffectNum++;
}

void EffectManager::Update()
{
	mManager->Update();

	// インスタンスチェック
	for (int i = 0; i < INST_MAX; ++i)
	{
		if (mInstHandles[i] == -1) continue;
		
		if (!mManager->Exists(mInstHandles[i]))
		{
			mInstHandles[i] = -1;
			mInstNum--;
		}
	}
}

void EffectManager::Render(const Matrix& view, const Matrix& proj)
{
	// ビュー プロジェクション行列更新
	Effekseer::Matrix44 viewEfk;
	memcpy(&viewEfk, &view, sizeof(Effekseer::Matrix44));
	mRenderer->SetCameraMatrix(viewEfk);

	Effekseer::Matrix44 projEfk;
	memcpy(&projEfk, &proj, sizeof(Effekseer::Matrix44));
	mRenderer->SetProjectionMatrix(projEfk);

	mRenderer->BeginRendering();
	mManager->Draw();
	mRenderer->EndRendering();
}

int EffectManager::Play(const int& slot, const Vector3& pos, int startFrame, float scale, float speed)
{
	if (!mEffects[slot].Get()) return -1;
	Effekseer::Handle h = mManager->Play(mEffects[slot], Effekseer::Vector3D(pos.x, pos.y, pos.z), startFrame);
	mManager->SetSpeed(h, speed);
	mManager->SetScale(h, scale, scale, scale);

	for (int i = 0; i < INST_MAX; ++i)
	{
		if (mInstHandles[i] != -1) continue;
		
		mInstHandles[i] = h;
		break;
	}

	return h;
}

