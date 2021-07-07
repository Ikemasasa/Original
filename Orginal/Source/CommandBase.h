#pragma once
#include "ItemData.h"

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
	const ItemData::ItemParam* mUseItemParam;

public:
	CommandBase() = default;
	virtual ~CommandBase() = default;
	virtual void Update(const BattleActorManager* bam) = 0;
	virtual void Render() const {};

	bool IsBehaviourEnable() const { return (mBehaviour != Behaviour::NONE); }
	void BehaviourFinished();

	int GetTargetObjID() const { return mTargetObjID; }
	Behaviour GetBehaviour() const { return mBehaviour; }
	const ItemData::ItemParam* GetItemParam() { return mUseItemParam; }
	
	void SetBehaviour(const Behaviour behaviour) { mBehaviour = behaviour; }
	void SetTargetObjID(const int targetObjID) { mTargetObjID = targetObjID; }
	void SetItemParam(const ItemData::ItemParam* param) { mUseItemParam = param; }
};
