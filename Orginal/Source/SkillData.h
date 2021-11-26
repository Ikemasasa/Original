#pragma once
#include <string>
#include <vector>

#include "EffectData.h"
#include "Sound.h"

class SkillData
{
	friend class DataBase;

	// é¿ë‘çÏê¨ã÷é~
	SkillData() = default;
	~SkillData() = default;

public:
	enum Type      { BUFF, DEBUFF, ATTACK };
	enum Target    { NONE, PARTY, ENEMY };
	enum Range { SINGLE, ALL };
	enum Rate      { VALUE, PERCENT };

	struct BaseData
	{
		std::wstring name;
		int id;
		int useMP;
		Type type;
		Target target;
		Range range;
		Rate rate;
		int effectID;
		int soundID;
		std::wstring info;
	};

	enum Kind
	{
		ATK_UP,
		DEF_DOWN
	};

private:
	static std::vector<BaseData> mSkills;
	static void Initialize();
	static void Release();

	static void LoadSkillData();

public:
	static const BaseData* GetBaseData(Kind kind) { return &mSkills[kind]; }
	static std::vector<const BaseData*> GetCharaSkill(int charaID);
};

class BuffSkillData
{
public:
	struct Param
	{
		const SkillData::BaseData* base;
		int turn;
		float atkValue;
		float defValue;
	};

public:
	static Param GetParam(const size_t id);
};

class AttackSkillData
{
public:
	struct Param
	{
		const SkillData::BaseData* base;
		float skillRate; // ÉXÉLÉãî{ó¶
		int hitNum;
	};

public:
	static Param GetParam(const size_t id);
};