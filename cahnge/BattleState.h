#pragma once
#include "Singleton.h"

class CameraBase;

class BattleState : public SingletonManual<BattleState>
{
public:
	static constexpr float BEGIN_TIME = 1.0f;

	enum class State
	{
		BEGIN,
		BEGIN_FINISHED,
		COMMAND_SELECT,
		PARTY_SELECT,
		ENEMY_SELECT,
		PLAYER_ATTACK,
		ENEMY_ATTACK,
		RESULT,
		MAX
	};

private:
	State mState = State::BEGIN;

public:
	BattleState() = default;
	~BattleState() = default;

	void SetState(State state) { mState = state; };
	State GetState() const { return mState; }
	bool CheckState(State state) { return mState == state; }
};