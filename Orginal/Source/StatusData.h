#pragma once
#include <string>
#include <vector>

#include "lib/Math.h"

#include "Equipment.h"

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
	Equipment equipments;

	int GetAtk() const;
	int GetDef() const;
	int GetSpd() const;

	bool IsDead() const { return hp <= 0; }
	bool IsFullHP() const { return hp == maxHP; }
	bool IsFullMP() const { return mp == maxMP; }

	void HealHP(const int healValue) { hp = Math::Min(hp + healValue, maxHP); }
	void HurtHP(const int hurtValue) { hp = Math::Clamp(hp - hurtValue, 0, maxHP); }
	void HealMP(const int healValue) { mp = Math::Min(mp + healValue, maxMP); }
	void HurtMP(const int hurtValue) { mp = Math::Clamp(mp - hurtValue, 0, maxMP); }
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
	//void SetPLStatus(const std::wstring& name, const Status& status);
};