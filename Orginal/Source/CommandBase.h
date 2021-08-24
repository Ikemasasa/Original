#pragma once
#include "ItemData.h"
#include "SkillData.h"

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
		GUARD,
		ESCAPE,

		MAX
	};

protected:
	Behaviour mBehaviour = Behaviour::NONE;
	std::vector<int> mTargetObjIDs;
	const ItemData::ItemParam* mUseItemParam    = nullptr;
	const SkillData::SkillParam* mUseSkillParam = nullptr;

public:
	CommandBase() = default;
	virtual ~CommandBase() = default;
	virtual void Update(const BattleCharacterManager* bcm) = 0;
	virtual void Render() const {};

	bool IsBehaviourEnable() const { return (mBehaviour != Behaviour::NONE); }
	virtual void BehaviourFinished();

	std::vector<int> GetTargetObjIDs() const { return mTargetObjIDs; }
	Behaviour GetBehaviour() const { return mBehaviour; }
	const ItemData::ItemParam* GetItemParam() const { return mUseItemParam; }
	const SkillData::SkillParam* GetSkillParam() const { return mUseSkillParam; }
	
	void SetBehaviour(const Behaviour behaviour) { mBehaviour = behaviour; }
	void SetItemParam(const ItemData::ItemParam* param) { mUseItemParam = param; }
	void SetSkillParam(const SkillData::SkillParam* param)  { mUseSkillParam = param; }
	void AddTargetObjID(const int targetObjID) { mTargetObjIDs.push_back(targetObjID); }
};
