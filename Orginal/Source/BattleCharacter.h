#pragma once
#include <memory>

#include "Character.h"
#include "Item.h"
#include "StatusData.h"

class CommandBase;
class BattleCharacterManager;

// 戦闘用のキャラクタークラス
class BattleCharacter : public Character
{
protected: // 変数
	std::unique_ptr<CommandBase> mCommand = nullptr; // コマンド決定クラス
	Status mStatus;									 // ステータス
	ItemInventory mInventory;						 // アイテムのインベントリ

public: // 関数

	// コンストラクタ
	BattleCharacter(const Character* chara, Status status);
	
	// デストラクタ
	virtual ~BattleCharacter() = default;

	// 初期化(純粋仮想)
	virtual void Initialize() = 0;

	// 更新
	virtual void Update(const BattleCharacterManager* bcm);
	
	// コマンド関係の画像とかを描画
	void RenderCommand() const;

	CommandBase* GetCommand() const { return mCommand.get(); } // コマンド決定クラスを取得
	Status* GetStatus() { return &mStatus; }				   // ステータスを取得
	ItemInventory* GetInventory() { return &mInventory; }	   // アイテムのインベントリを取得
};