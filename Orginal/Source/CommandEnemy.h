#pragma once
#include "CommandBase.h"

// 普通の敵のコマンド決定クラス
class CommandEnemy : public CommandBase
{
public:

	// コンストラクタ
	CommandEnemy() = default;

	// デストラクタ
	~CommandEnemy() = default;

	// 更新
	void Update(const BattleCharacterManager* bcm) override;
};