#pragma once
#include "Singleton.h"

class CameraBase;

class BattleState : public SingletonManual<BattleState>
{
public:
	enum class State
	{
		SORT_ORDER,
		COMMAND_SELECT,
		PARTY_SELECT,
		ENEMY_SELECT,
		RESULT,
		MAX
	};

private:
	State mState = State::SORT_ORDER;

public:
	BattleState() = default;
	~BattleState() = default;

	void SetState(State state) { mState = state; };
	State GetState() const { return mState; }
};