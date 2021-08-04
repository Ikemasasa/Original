#pragma once
#include "ItemData.h"

class BattleCharacterManager;

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
	Behaviour mBehaviour = Behaviour::NONE;
	int mTargetObjID = -1;
	int mUseItemIndex = -1;

public:
	CommandBase() = default;
	virtual ~CommandBase() = default;
	virtual void Update(const BattleCharacterManager* bcm) = 0;
	virtual void Render() const {};

	bool IsBehaviourEnable() const { return (mBehaviour != Behaviour::NONE); }
	void BehaviourFinished();

	int GetTargetObjID() const { return mTargetObjID; }
	Behaviour GetBehaviour() const { return mBehaviour; }
	int GetItemIndex() const { return mUseItemIndex; }
	
	void SetBehaviour(const Behaviour behaviour) { mBehaviour = behaviour; }
	void SetTargetObjID(const int targetObjID) { mTargetObjID = targetObjID; }
	void SetItemIndex(const int index)  { mUseItemIndex = index; }
};
