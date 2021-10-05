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

	bool guardFlag;

	// 装備品
	Equipment equipments;

	// バフデバフ量
	BuffData buffAtk;
	BuffData buffDef;
	BuffData debuffAtk;
	BuffData debuffDef;

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

	int GetStr() const { return str; }
	int GetVit() const { return vit; }
	int GetAgi() const { return agi; }

	int GetAtk() const;
	int GetDef() const;
	int GetSpd() const;

	float GetBuffAtkRate() const { return buffAtk.rate; }
	float GetBuffDefRate() const { return buffDef.rate; }
	float GetDebuffAtkRate() const { return debuffAtk.rate; }
	float GetDebuffDefRate() const { return debuffDef.rate; }

	Equipment* GetEquipments() { return &equipments; }

	// セッター
	void SetHP(const int value) { hp = Math::Clamp(value, 0, maxHP); }
	void SubHP(const int value) { hp = Math::Max(0, hp - value);     }
	void AddHP(const int value) { hp = Math::Min(maxHP, hp + value); }

	void SetMP(const int value) { mp = Math::Clamp(value, 0, maxMP); }
	void SubMP(const int value) { mp = Math::Max(0, mp - value);     }
	void AddMP(const int value) { mp = Math::Min(maxMP, mp + value); }

	void SetBuffAtkRate(const float rate, const int turn);
	void SetBuffDefRate(const float rate, const int turn);
	void SetDebuffAtkRate(const float rate, const int turn);
	void SetDebuffDefRate(const float rate, const int turn);
	void SetGuardFlag(const bool flag) { guardFlag = flag; }
	bool GetGuardFlag() { return guardFlag; }
	void AdvanceBuffTurn();
	void ResetBuff();

};

class StatusData
{
	friend class DataBase;

	// 実態作成禁止
	StatusData();
	~StatusData();

public:
	enum EnemyType
	{
		MOB,
		BOSS,

		NONE
	};

private:
	static std::vector<Status> mPLStatus;

private:
	static void LoadPLStatus();
	static void Initialize();
	static void Release();

public:
	static Status GetPLStatus(size_t id);
	static Status GetEnmStatus(size_t id);
	static EnemyType GetEnmType(size_t id);

	static void SetPLStatus(size_t charaID, const Status& status);
};