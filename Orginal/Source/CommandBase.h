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
		GUARD,
		ATTACK,
		SKILL,
		ESCAPE,

		MAX
	};

protected:
	Behaviour mBehaviour = Behaviour::NONE;
	std::vector<int> mTargetObjIDs;

	bool mIsUseItem = false;
	UseItemData::Param mUseItemParam;

	bool mIsUseSkill = false;
	SkillData::SkillParam mUseSkillParam;

public:
	CommandBase() = default;
	virtual ~CommandBase() = default;
	virtual void Update(const BattleCharacterManager* bcm) = 0;
	virtual void Render() const {};

	bool IsUseItem() const { return mIsUseItem; }
	bool IsUseSkill() const { return mIsUseSkill; }
	bool IsBehaviourEnable() const { return (mBehaviour != Behaviour::NONE); }
	virtual void BehaviourFinished();

	std::vector<int> GetTargetObjIDs() const { return mTargetObjIDs; }
	Behaviour GetBehaviour() const { return mBehaviour; }
	const UseItemData::Param* GetItemParam() const { return &mUseItemParam; }
	const SkillData::SkillParam* GetSkillParam() const { return &mUseSkillParam; }
	
	void SetBehaviour(const Behaviour behaviour) { mBehaviour = behaviour; }
	void SetItemParam(const UseItemData::Param* param);
	void SetSkillParam(const SkillData::SkillParam* param);
	void AddTargetObjID(const int targetObjID) { mTargetObjIDs.push_back(targetObjID); }
};
