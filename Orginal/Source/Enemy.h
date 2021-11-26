#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Character.h"
#include "StatusData.h"

// フィールドのボスキャラクタークラス
class Enemy : public Character
{
	// 定数
	static constexpr float WAIT_TO_WALK_SEC = 7.0f; // 待機から歩きまでの間隔
	static constexpr float WALK_TO_WAIT_SEC = 3.0f; // 歩きから待機までの間隔
	static constexpr float WALK_SPEED = 0.08f;		// 歩きのスピード
	static constexpr float RUN_SPEED = WALK_SPEED * 2.0f; // 走りのスピード
	static const int MASS = 0;	     // 敵の質量
private: // 変数
	enum MoveState
	{
		WAIT,
		WALK,
		CHASE,

		MAX
	};

	MoveState mState = WAIT;		// 動きのステート
	StatusData::EnemyType mEnmType; // 敵のタイプ
	float mTimer = 0;				// タイマー
	float mMoveAngle;				// 歩きの時の角度

private: // 関数
	
	// プレイヤーを追いかけるステートに入るかチェック
	bool IsChase(const Vector3& playerPos);

	// 移動更新
	void Move(const Vector3& playerPos);
	
	// 待機ステート
	void StateWait();

	// 歩きステート
	void StateWalk();

	// 追いかけステート
	void StateChase(const Vector3& playerPos);
public:

	// コンストラクタ
	Enemy(int charaID);

	// デストラクタ
	virtual ~Enemy();

	// 初期化
	virtual void Initialize();

	// 更新
	virtual void Update(const Vector3& playerPos);
	
	// 敵のタイプを取得
	StatusData::EnemyType GetEnmType() const { return mEnmType; }
	
	// 敵のタイプを設定
	void SetEnmType(const StatusData::EnemyType& type) { mEnmType = type; }
};