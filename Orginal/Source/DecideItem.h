#pragma once
#include "IDecideCommand.h"

#include "ItemSelect.h"

/*
	戦闘時のコマンドでアイテムを選んだ後の処理をするクラス
	もってるアイテムの中から選択する
*/
class DecideItem : public IDecideCommand
{
	ItemSelect mItemSelect; // アイテム選択クラス

public: // 関数

	// コンストラクタ
	DecideItem() = default;

	// デストラクタ
	~DecideItem() = default;

	// 初期化
	void Initialize(const BattleCharacterManager* bcm) override;

	// 更新
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;

	// 描画
	void Render();
};