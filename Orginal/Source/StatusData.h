#pragma once
#include <string>
#include <vector>

#include "lib/Math.h"

struct Status
{
	std::wstring name; // –¼‘O
	int id;			  // ID
	int hp;			  // HP
	int maxHP;
	int mp;			  // MP
	int maxMP;
	int str;		  // Šî‘bUŒ‚—Í
	int vit;		  // Šî‘b–hŒä—Í
	int agi;		  // ‘f‘‚³

	// ‘•”õ•i
	int equipAtk; // UŒ‚—Í
	int equipDef; // –hŒä—Í
	int equipSpd; // ‘f‘‚³

	int GetTotalAtk() const { return str + equipAtk; }
	int GetTotalDef() const { return vit + equipDef; }
	int GetTotalSpd() const { return agi + equipSpd; }

	bool IsDead() const { return hp <= 0; }
	bool IsFullHP() const { return hp == maxHP; }
	bool IsFullMP() const { return mp == maxMP; }

	void HealHP(const int healValue) { hp = Math::Min(hp + healValue, maxHP); }
	void HurtHP(const int hurtValue) { hp = Math::Max(hp - hurtValue, maxHP); }
	void HealMP(const int healValue) { mp = Math::Min(mp + healValue, maxMP); }
	void HurtMP(const int hurtValue) { mp = Math::Max(mp - hurtValue, maxMP); }
};

class StatusData
{
	std::vector<Status> mPLStatus;
	std::vector<Status> mEnmStatus;

	void LoadPLStatus();
	void LoadEnmStatus();

public:
	StatusData();
	~StatusData();

	void Initialize();
	Status GetPLStatus(size_t id) const;
	Status GetEnmStatus(size_t id) const;

	void SetPLStatus(size_t charaID, const Status& status);
};