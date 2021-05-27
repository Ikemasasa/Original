#pragma once

class BattleActorManager;

class CommandBase
{
public:
	enum class Behaviour
	{
		NONE,
		USE_ITEM,
		SKILL,
		ATTACK,
		DEFENCE,
		ESCAPE,

		MAX
	};

protected:
	Behaviour mBehaviour;
	int mTargetObjID;

public:
	CommandBase() = default;
	virtual ~CommandBase() = default;
	virtual void Update(const BattleActorManager* bam) = 0;

	int GetTargetObjID() const { return mTargetObjID; }
	Behaviour GetBehaviour() const { return mBehaviour; }
	void BehaviourFinished() { mBehaviour = Behaviour::NONE; }
	bool IsBehaviourEnable() { return (mBehaviour != Behaviour::NONE); }
};
