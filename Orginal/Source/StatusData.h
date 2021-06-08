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
	
	// ƒoƒtŒø‰Ê“K‰ž—p
	int buffAtk; // UŒ‚—Í‚Ìƒoƒt—Ê
	int buffDef; // –hŒä—Í‚Ìƒoƒt—Ê
	int buffSpd; // ‘f‘‚³‚Ìƒoƒt—Ê

	int GetTotalAtk() const { return str + equipAtk + buffAtk; }
	int GetTotalDef() const { return vit + equipDef + buffDef; }
	int GetTotalSpd() const { return agi + equipSpd + buffSpd; }

	bool IsAlive() const { return hp > 0; }
	bool IsFullHP() const { return hp == maxHP; }
	bool IsFullMP() const { return mp == maxMP; }

	void SetHP(const int hpValue) { hp = Math::Min(hpValue, maxHP); }
	void HealHP(const int healValue) { hp = Math::Min(hp + healValue, maxHP); }
	void SetMP(const int mpValue) { mp = Math::Min(mpValue, maxMP); }
	void HealMP(const int healValue) { mp = Math::Min(mp + healValue, maxMP); }
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