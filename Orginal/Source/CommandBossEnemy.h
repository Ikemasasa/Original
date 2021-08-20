#pragma once
#include "CommandBase.h"

class BattleCharacterManager;

class CommandBossEnemy : public CommandBase
{
	enum class State
	{
		TARGET_ONE,
		TARGET_TWO,
		
		MAX
	};

	State mState = State::TARGET_ONE;

	// lowHPのキャラのCharaIDを返す
	std::vector<int> SearchLowHPChara(const int num, const BattleCharacterManager* bcm);
public:
	CommandBossEnemy();
	~CommandBossEnemy();

	void Update(const BattleCharacterManager* bcm) override;
};