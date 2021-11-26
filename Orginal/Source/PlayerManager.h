#pragma once
#include <vector>

#include "EquipmentInventory.h"
#include "Item.h"

class CameraBase;
class Matrix;
class Player;
class Shader;
class Vector4;

/*
	プレイヤーマネージャ
	プレイヤーの生成、更新、描画をしている
*/
class PlayerManager
{
	// 定数
	static const int INVINCIBLE_SECOND = 2; // 無敵時間

public:
	enum PlList
	{
		ERIKA = DataBase::PL_ID_START,
		PALADIN,

		PL_ID_MAX
	};
	static const int ALL_PLAYER_NUM = PL_ID_MAX - DataBase::PL_ID_START;

private: // 変数
	std::vector<std::unique_ptr<Player>> mPlayers; // プレイヤーのリスト
	Player* mMovePlayer = nullptr;			       // フィールドで動くプレイヤー
	bool mIsInvincible = false;					   // 無敵時間かどうか
	float mInvincibleTimer = 0;					   // 無敵時間のタイマー

	EquipmentInventory mEquipmentInventory;		   // 装備品のインベントリ

public: // 関数

	// コンストラクタ
	PlayerManager();

	//デストラクタ
	~PlayerManager();

	// 生成
	void Create(int charaID);

	// 初期化
	void Initialize();

	// 更新
	void Update(const bool isTalking);

	// 描画
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// ゲッター
	Player* GetMovePlayer() const { return mMovePlayer; }
	Player* GetPlayer(const size_t index) const { return mPlayers[index].get(); }
	size_t GetNum() const { return mPlayers.size(); }
	bool IsInvincible() const { return mIsInvincible; }
	EquipmentInventory* GetEquipmentInventory() { return &mEquipmentInventory; }

	// セッター
	void EnableInvincible() { mIsInvincible = true; }

};