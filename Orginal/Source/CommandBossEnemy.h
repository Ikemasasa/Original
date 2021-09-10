#pragma once
#include "CommandBase.h"

class BattleCharacterManager;

class CommandBossEnemy : public CommandBase
{
	static constexpr float ATTACK_LOWHP_CHARA_PERCENT = 30.0f;

	enum class State
	{
		TARGET_ONE,
		TARGET_TWO,
		
		MAX
	};

	State mState = State::TARGET_ONE;

	// lowHPÇÃÉvÉåÉCÉÑÅ[ÇÃCharaIDÇï‘Ç∑
	std::vector<int> SearchLowHPPlayer(const size_t num, const BattleCharacterManager* bcm);
	std::vector<int> GetRandomPlayerObjIDs(const size_t num, const BattleCharacterManager* bcm);
public:
	CommandBossEnemy();
	~CommandBossEnemy();

	void Update(const BattleCharacterManager* bcm) override;
};