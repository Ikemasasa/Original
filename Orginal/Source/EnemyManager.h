#pragma once
#include <list>

#include "lib/Vector.h"

#include "DataBase.h"

class Enemy;
class Shader;

//
/*
	エネミーマネージャクラス
	敵の生成、更新、描画をしている
*/
class EnemyManager
{
public:
	enum EnmList
	{
		MONSTER_A = DataBase::ENM_ID_START,
		MONSTER_B,
		BOSS_MONSTER,

		ENM_NUM
	};

private: // 変数
	std::list<std::unique_ptr<Enemy>> mEnemies; // 敵リスト
	Vector3 mPlayerPos = {};					// プレイヤー座標

public: // 関数

	// コンストラクタ
	EnemyManager();

	// デストラクタ
	~EnemyManager();

	// 敵を生成
	void Create(int charaID); 

	// 敵を削除
	std::list<std::unique_ptr<Enemy>>::iterator Destroy(int objID);

	// 初期化
	void Initialize();

	// 更新
	void Update(const bool isTalking);
	
	// 描画
	void Render(const Shader* shader, const Matrix& view, const Matrix& projection, const Vector4& lightDir);

	// 敵の数を取得
	int GetNum() const { return mEnemies.size(); }
	
	// 引数のIDの敵を取得
	Enemy* GetEnemy(int objID);

	// プレイヤー座標を設定
	void SetPlayerPos(const Vector3& pos) { mPlayerPos = pos; }
};