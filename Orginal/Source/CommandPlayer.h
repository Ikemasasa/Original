#pragma once
#include <memory>
#include <stack>

#include "CommandBase.h"

class IDecideCommand;
class Sprite;

// プレイヤーのコマンド決定クラス
class CommandPlayer : public CommandBase
{
private: // 変数
	std::stack<std::unique_ptr<IDecideCommand>> mCommand; // コマンド選択のスタック

public: // 関数

	// コンストラクタ
	CommandPlayer();

	// デストラクタ
	~CommandPlayer();

	// 更新
	void Update(const BattleCharacterManager* bcm) override;
	
	// 描画
	void Render() const override;

	// 行動終了
	void BehaviourFinished() override;
};