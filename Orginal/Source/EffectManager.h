#pragma once
#include <DirectXMath.h>

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "lib/Vector.h"

class CameraBase;

/*
	エフェクトの管理を行うクラス
	色々なところから呼び出したいのでSingletonを通す
*/
class EffectManager
{
	// 定数
	static const int SQUARE_MAX = 8000; // 適当
	static const int EFFECT_MAX = 256; // 適当
	static const int INST_MAX = 512;

private:// 変数
	Effekseer::ManagerRef mManager;				  // マネージャ
	EffekseerRendererDX11::RendererRef mRenderer; // レンダラー

	Effekseer::EffectRef mEffects[EFFECT_MAX] = { nullptr }; // エフェクトの元データ
	int mEffectNum = 0;										 // 元データの数

	Effekseer::Handle mInstHandles[INST_MAX] = {-1}; // エフェクトのインスタンス
	int mInstNum = 0;								 // インスタンスの数

public:// 関数

	// コンストラクタ
	EffectManager();

	// デストラクタ
	~EffectManager();

	// 初期化関数
	void Initialize();

	// エフェクトの元データ作成 / 戻り値 : slot
	int Create(const EFK_CHAR* efkPath); 
	void Create(const EFK_CHAR* efkPath, int slot);

	// 更新関数(エフェクトのフレームの更新とかしてると思う)
	void Update();

	// 描画関数
	void Render(const Matrix& view, const Matrix& proj);

	// エフェクト再生リストに追加 / 戻り値 ： インスタンスハンドル
	int Play(const int& slot, const Vector3& pos, int startFrame = 0, float scale = 1.0f, float speed = 1.0f);

	// エフェクトを停止
	void Stop(const int instHandle);

	// 引数のエフェクトが再生されているか
	bool IsPlay(const int instHandle) const;
};