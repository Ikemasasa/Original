#pragma once
#include <DirectXMath.h>
#include <memory>

#include "EnemyManager.h"
#include "NPCManager.h"
#include "NPCTalk.h"
#include "PlayerManager.h"

class Character;
class Player;
class Shader;

// キャラクターのマネージャクラス
class CharacterManager
{
public: // 定数




private: // 変数
	std::unique_ptr<PlayerManager> mPlayerManager; // プレイヤーマネージャ
	std::unique_ptr<EnemyManager> mEnemyManager;   // エネミーマネージャ
	std::unique_ptr<NPCManager> mNPCManager;	   // NPCマネージャ
	std::unique_ptr<NPCTalk> mNPCTalk;			   // NPCと会話する処理をするクラス
	Enemy* mHitEnemy = nullptr;					   // フィールドで接敵した敵のシンボル

private: // 関数
	
	// 押し合い
	void Collide(Character* a, Character* b);

	// 押し出し
	void CollideObject(Character* move, Character* object);

public: 

	// コンストラクタ
	CharacterManager();
	
	// デストラクタ
	~CharacterManager() = default;

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);
	
	// UIを描画
	void RenderUI();

	// NPCとの衝突判定
	void CollideNPC();

	// 敵との衝突判定
	void CollideEnemy();

	// NPCと会話ができるかチェックする
	void TalkCheck();

	// 接敵した敵がいるかどうかを取得
	Enemy* IsHitEnemy() const { return mHitEnemy; }

	// 設定した敵の変数をリセット
	void ResetHitEnemy() { mHitEnemy = nullptr; }

	// フィールドで動かすプレイヤーを取得
	Player* GetMovePlayer() const { return mPlayerManager->GetMovePlayer(); }

	// プレイヤーマネージャを取得
	PlayerManager* GetPlayerManager() const { return mPlayerManager.get(); }

	// エネミーマネージャを取得
	EnemyManager* GetEnemyManager() const { return mEnemyManager.get(); }
};