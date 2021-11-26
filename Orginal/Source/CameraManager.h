#pragma once
#include <stack>
#include <memory>

#include "lib/Vector.h"
#include "lib/Matrix.h"

#include "CameraBase.h"

// 前方宣言
class Character;
class BattleCharacter;
class BattleCharacterManager;


/*
	カメラマネージャー
	カメラの情報を色々なところからとるために作った
	なのでSingletonクラスを通すこと
*/
class CameraManager
{
	// 変数

	// SceneManagerがstack使ってるからカメラも一応
	std::stack<std::unique_ptr<CameraBase>> mMainCamera;

public: // 関数

	// コンストラクタ
	CameraManager();

	// デストラクタ
	~CameraManager();
	
	// カメラをマネージャに追加
	void Push(std::unique_ptr<CameraBase> camera) { mMainCamera.emplace(camera.release()); };

	// カメラをマネージャから削除
	void Pop() { if(!mMainCamera.empty()) mMainCamera.pop(); }

	// 初期化関数
	void Initialize(const Vector3& target, float distance = 5.0f);

	// 更新関数
	void Update(const Vector3& target);
	void Update(const Character* target);
	void Update(const BattleCharacter* target, const BattleCharacterManager* bcm);

	// 揺らしのパラメータ設定関数
	void Shake(float duration, float strength);

	// ゲッター
	Matrix GetView() const;		  // ビュー行列取得
	Matrix GetProj() const;		  // プロジェクション行列取得	
	Vector3 GetPos() const;		  // 座標取得
	float GetZenithAngle() const;  // 天頂角(縦)取得
	float GetAzimuthAngle() const; // 方位角(横)取得
	Vector3 GetForward() const;	  // 前方ベクトル取得
	Vector3 GetForwardXZ() const;  // XZ平面の前方ベクトル取得
	Vector3 GetRight() const;      // 右方ベクトル取得
	Vector3 GetRightXZ() const;    // XZ平面の右方ベクトル取得
};