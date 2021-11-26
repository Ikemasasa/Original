#pragma once

#include "Character.h"
#include "Item.h"


/*
	フィールドのプレイヤーキャラクタークラス
	主に動きの処理をしている
*/
class Player : public Character
{
	// 定数
	static constexpr float RUN_SPEED = 0.28f;		  // 走りのスピード
	static constexpr float RAY_DOWN_DIST = 0.5f;	  // 下方向のレイの距離
	static constexpr float POS_Y_LERP_FACTOR = 0.25f; // Y座標の補正の係数
	static const int MASS = 100;		 // プレイヤーの質量

private: // 変数
	ItemInventory mInventory; // アイテムのインベントリ

public: // 関数
	
	// コンストラクタ
	Player(int charaID);

	// デストラクタ
	~Player() = default;

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// アイテムのインベントリを取得
	ItemInventory* GetInventory() { return &mInventory; }

	// アイテムのインベントリを設定
	void SetInventory(const ItemInventory* inventory) { mInventory = *inventory; }
};