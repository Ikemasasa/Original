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
		ESCAPE,
		RESULT,
		MAX
	};

private:
	State mState = State::BEGIN;
	State mOldState = State::BEGIN;

public:
	BattleState() = default;
	~BattleState() = default;

	void SetState(State state) { mOldState = mState; mState = state; };
	State GetState() const { return mState; }
	State GetOldState() const { return mOldState; }
	bool CheckState(State state) { return mState == state; }
	bool CheckOldState(State state) { return mOldState == state; }

};