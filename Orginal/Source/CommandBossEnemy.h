#pragma once
#include "CommandBase.h"

class BattleCharacterManager;

// ボスのコマンド決定クラス
class CommandBossEnemy : public CommandBase
{
	// 定数
	static constexpr float ATTACK_LOWHP_CHARA_PERCENT = 30.0f; // 低HPのキャラを狙う確率

private: // 変数
	enum class State
	{
		TARGET_ONE,
		TARGET_TWO,
		
		MAX
	};
	State mState = State::TARGET_ONE; // ターゲットの数


private: // 関数

	// lowHPのプレイヤーのCharaIDを返す
	std::vector<int> SearchLowHPPlayer(const size_t num, const BattleCharacterManager* bcm);

	// ランダムのプレイヤーIDを取得
	std::vector<int> GetRandomPlayerObjIDs(const size_t num, const BattleCharacterManager* bcm);

public: 
	
	// コンストラクタ
	CommandBossEnemy();

	//デストラクタ
	~CommandBossEnemy();

	// 更新
	void Update(const BattleCharacterManager* bcm) override;
};