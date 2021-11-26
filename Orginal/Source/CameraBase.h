#pragma once
#include <memory>

#include "lib/Matrix.h"
#include "lib/Vector.h"

// 前方宣言
class Character;
class BattleCharacter;
class BattleCharacterManager;


// カメラの基底クラス(必要そうなものを詰め込んだ)
class CameraBase
{
protected: // 変数

	// ビュー関係
	Vector3 mPos		= {};	// 座標
	Vector3 mTarget		= {};	// 注視点
	float mZenithAngle  = 0.0f; // 天頂角:rad(縦の角度)
	float mAzimuthAngle = 0.0f; // 方位角:rad(横の角度)
	Matrix mView	    = {};   // ビュー行列

	// プロジェクション関係
	float mFov = 0.0f;		 // 視野角
	float mAspect = 0.0f;	 // アスペクト比
	float mNear = 0.0f;		 // ニアクリップ距離
	float mFar = 0.0f;		 // ファークリップ距離
	Matrix mProjection = {}; // プロジェクション行列

	// カメラの方向のベクトル
	Vector3 mForward = {};
	Vector3 mRight = {};

	// 揺らし関係
	bool mIsShake = false;		 // 揺らし中かのフラグ
	float mShakeDuration = 0.0f; // 揺らしの時間
	float mShakeStrength = 0.0f; // 揺らしの強さ
	float mShakeElapsed = 0.0f;  // 揺らしの経過時間


protected: // 関数

	// ビュー行列更新
	void UpdateView();

	// 揺らし更新
	void UpdateShake(float elapsedTime);

	// 角度から座標を算出
	Vector3 CalcPosFromAngle(float distance);

public:
	// コンストラクタ
	CameraBase(); 

	// デストラクタ
	virtual ~CameraBase() = default;

	// 初期化関数
	virtual void Initialize(const Vector3& target, float distance = 5.0f);

	// 更新関数
	virtual void Update(const Vector3& target) {};
	virtual void Update(const Character* target) {};
	virtual void Update(const BattleCharacter* commandChara, const BattleCharacterManager* bcm) {};

	// 揺らしのパラメータ設定関数
	void Shake(float duration, float strength);

	//	平行投影行列設定関数
	void SetOrthoMatrix(float w, float h, float znear, float zfar);					

	//	透視投影行列設定関数
	void SetPerspectiveMatrix(float fov, float aspect, float znear, float zfar);	

	// ゲッター
	Matrix GetViewMatrix() const { return mView; }				// ビュー行列取得関数
	Matrix GetProjectionMatrix() const { return mProjection; }	// 投影行列取得関数
	Vector3 GetPos() const { return mPos; }						// 座標取得
	float GetZenithAngle() const { return mZenithAngle; }		// 天頂角取得
	float GetAzimuthAngle() const { return mAzimuthAngle; }		// 方位角取得
	Vector3 GetForward() const { return mForward; }				// 前方ベクトル取得
	Vector3 GetForwardXZ() const { return Vector3(mForward.x, 0.0f, mForward.z).GetNormalize(); } // XZ平面の前方ベクトル取得
	Vector3 GetRight() const { return mRight; }					// 右方ベクトル取得
	Vector3 GetRightXZ() const { return Vector3(mRight.x, 0.0f, mRight.z).GetNormalize(); } // XZ平面の右方ベクトル取得
};