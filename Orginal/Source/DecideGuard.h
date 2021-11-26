#pragma once
#include "IDecideCommand.h"

#include "SelectOptions.h"


/*
	戦闘時のコマンドで逃げるを選んだ後の処理をするクラス
	 中身的には「本当にガードをしますか？」みたいなことをしてるだけ
*/
class DecideGuard : public IDecideCommand
{
	// 定数
	static constexpr float BOARD_POS_X = 200.0f; // 選択肢の画像のX座標
	static constexpr float BOARD_POS_Y = 220.0f; // 選択肢の画像のY座標
	SelectOptions mSelectOptions;				 // 選択肢を選ぶクラス


public: // 関数

	// コンストラクタ
	DecideGuard() = default;

	// デストラクタ
	~DecideGuard() = default;

	// 初期化
	void Initialize(const BattleCharacterManager* bcm) override;

	// 更新
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;

	// 描画
	void Render() override;
};