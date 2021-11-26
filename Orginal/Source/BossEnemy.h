 #pragma once
#include "Enemy.h"

// フィールドのボスキャラクタークラス
class BossEnemy : public Enemy
{
	// 定数
	static constexpr float MASS = 1000.0f; // 質量

public: // 関数

	// コンストラクタ
	BossEnemy(int charaID);

	// デストラクタ
	~BossEnemy();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const Vector3& playerPos) override;
};