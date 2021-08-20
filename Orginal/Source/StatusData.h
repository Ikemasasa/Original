#pragma once
#include <string>
#include <vector>

#include "lib/Math.h"

#include "Equipment.h"

struct Status
{
	friend class StatusData;

public:
	struct BuffData
	{
		float rate = 1.0f;
		int turn = 0;
	};

private:
	std::wstring name; // 名前
	int id;			  // ID
	int hp;			  // HP
	int maxHP;
	int mp;			  // MP
	int maxMP;
	int str;		  // 基礎攻撃力
	int vit;		  // 基礎防御力
	int agi;		  // 素早さ

	// 装備品
	Equipment equipments;

	// バフデバフ量
	BuffData buffStr;
	BuffData buffVit;

public:	

	// 簡易チェック
	bool IsDead() const { return hp <= 0; }
	bool IsFullHP() const { return hp == maxHP; }
	bool IsFullMP() const { return mp == maxMP; }

	// ゲッター
	std::wstring GetName() const { return name; }
	//int GetID() const { return id; }
	int GetID()    const { return id; }
	int GetHP()    const { return hp; }
	int GetMaxHP() const { return maxHP; }
	int GetMP()    const { return mp; }
	int GetMaxMP() const { return maxMP; }

	int GetAtk() const;
	int GetDef() const;
	int GetSpd() const;

	Equipment* GetEquipments() { return &equipments; }

	// セッター
	void SetHP(const int value) { hp = Math::Clamp(value, 0, maxHP); }
	void SubHP(const int value) { hp = Math::Max(0, hp - value);     }
	void AddHP(const int value) { hp = Math::Min(maxHP, hp + value); }

	void SetMP(const int value) { mp = Math::Clamp(value, 0, maxMP); }
	void SubMP(const int value) { mp = Math::Max(0, mp - value);     }
	void AddMP(const int value) { mp = Math::Min(maxMP, mp + value); }

	void SetBuffStrRate(const float rate, const int turn);
	void SetBuffVitRate(const float rate, const int turn);
	void AdvanceBuffTurn();
};

class StatusData
{
	std::vector<Status> mPLStatus;
	std::vector<Status> mEnmStatus;

	void LoadPLStatus();
	void LoadEnmStatus();

public:
	enum EnemyType
	{
		MOB,
		BOSS,

		NONE
	};

public:
	StatusData();
	~StatusData();

	void Initialize();
	Status GetPLStatus(size_t id) const;
	Status GetEnmStatus(size_t id) const;
	EnemyType GetEnmType(size_t id) const;


	void SetPLStatus(size_t charaID, const Status& status);
};