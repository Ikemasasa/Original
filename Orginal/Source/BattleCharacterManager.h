#pragma once
#include <DirectXMath.h>
#include <memory>
#include <queue>
#include <vector>

#include "BattleCharacter.h"
#include "DropItemShower.h"
#include "SceneBattle.h"

// 前方宣言
class Enemy;
class Player;
class PlayerManager;
class TurnManager;
class Matrix;

// 戦闘用のキャラクターマネージャクラス
class BattleCharacterManager
{
	// 定数
	static const int BATTLECHARA_MAX = 12; // 戦闘キャラの最大数(特に意味はない)
	static const int BATTLECHARA_KIND = 2; // 戦闘キャラの種類(プレイヤ、敵)

public:
	static constexpr float PLAYER_POS_Z = -5.0f; // プレイヤーのZ座標
	static constexpr float ENEMY_POS_Z = 5.0f;   // 敵のZ座標

	static constexpr float POS_MIN_X = -5.0f; // X座標の最低値
	static constexpr float POS_MAX_X =  5.0f; // X座標の最高値


private: // 変数
	std::vector<std::shared_ptr<BattleCharacter>> mBCharacters; // 戦闘用キャラリスト
	std::vector<int> mAliveObjIDs[BATTLECHARA_KIND];			// 生存しているキャラのObjIDリスト
	std::vector<int> mDropItemIDs;								// ドロップアイテムのIDリスト
	BattleCharacter* mMoveChara = nullptr;						// 現在コマンド選択中のキャラクター
	int mPlayerNum;												// プレイヤーの数

	DropItemShower mDropItemShower;								// ドロップアイテム表示クラス

private: // 関数

	// プレイヤーをリストに追加
	void PlayerCreateAndRegister(Player* pl);

	// 敵をリストに追加
	void EnemyCreateAndRegister(Enemy* enm);
public:	

	// コンストラクタ
	BattleCharacterManager(PlayerManager* pm, Enemy* enemy);

	// デストラクタ
	~BattleCharacterManager() = default;

	// 初期化
	void Initialize();

	// 更新
	void Update(const TurnManager* turnManager);

	// 描画
	void Render(const Shader* shader, const Matrix& view, const Matrix& projection, const Vector4& lightDir);

	// UI描画
	void RenderUI();


	// キャラクターを整理、体力が0になったキャラをmAliveObjIDsから消す
	void OrganizeCharacter();

	// mAliveObjIDsから、バトル終了の判定をする
	SceneBattle::Result CheckBattleFinish(); 

	// ゲッター
	// 現在コマンド選択中のキャラを取得
	BattleCharacter* GetMoveChara() const { return mMoveChara; }

	// 生きてるキャラクターリストを取得
	const std::vector<int>& GetAliveObjIDs(Character::Type kind) const { return mAliveObjIDs[kind]; }

	// 引数のIDのキャラクタ―を
	BattleCharacter* GetChara(int objectID) const { return mBCharacters[objectID].get(); }

	// キャラクタ―リストを取得
	const std::vector<std::shared_ptr<BattleCharacter>>& GetBCharacters() const { return mBCharacters; }

	// ドロップアイテムのIDリストを取得
	const std::vector<int>& GetDropItemIDs() const { return mDropItemIDs; }
};