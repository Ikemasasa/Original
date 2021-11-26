#pragma once
#include "IDecideCommand.h"

#include <vector>

#include "SkillSelect.h"

/*
	戦闘時のコマンドでスキルを選んだ後の処理をするクラス
	 使うスキルを選択する
*/
class DecideSkill : public IDecideCommand
{
	// 定数
	static constexpr float BOARD_POS_X = 300; // スキルリストを表示するボードのX座標
	static constexpr float BOARD_POS_Y = 200; // スキルリストを表示するボードのY座標
	SkillSelect mSkillSelect;			      // スキル選択クラス

public: // 関数

	// コンストラクタ
	DecideSkill() = default;

	// デストラクタ
	~DecideSkill() = default;

	// 初期化
	void Initialize(const BattleCharacterManager* bcm) override;

	// 更新
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;

	// 描画
	void Render();
};