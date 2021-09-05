#pragma once
#include <string>
#include <vector>

class SkillData
{
	// é¿ë‘çÏê¨ã÷é~
	SkillData() = default;
	~SkillData() = default;

public:
	enum Type      { BUFF, DEBUFF };
	enum Target    { NONE, PARTY, ENEMY };
	enum TargetNum { SINGLE, ALL };
	enum Rate      { VALUE, PERCENT };

	struct SkillParam
	{
		std::wstring skillName;
		int skillID;
		int useMP;
		int turn;
		Type type;
		Target target;
		TargetNum targetNum;
		Rate rate;
		float atkValue;
		float defValue;
		std::wstring info;
	};

public:
	static std::vector<SkillParam> GetAllSkill(const int charaID);
};