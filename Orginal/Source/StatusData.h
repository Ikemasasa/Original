#pragma once
#include <string>
#include <vector>

#include "lib/Math.h"

struct Status
{
	std::wstring name; // ���O
	int id;			  // ID
	int hp;			  // HP
	int maxHP;
	int mp;			  // MP
	int maxMP;
	int str;		  // ��b�U����
	int vit;		  // ��b�h���
	int agi;		  // �f����

	// �����i
	int equipAtk; // �U����
	int equipDef; // �h���
	int equipSpd; // �f����
	
	// �o�t���ʓK���p
	int buffAtk; // �U���͂̃o�t��
	int buffDef; // �h��͂̃o�t��
	int buffSpd; // �f�����̃o�t��

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