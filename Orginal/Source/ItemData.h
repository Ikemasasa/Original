#pragma once
#include <memory>
#include <string>
#include <vector>

#include "DataBase.h"
#include "IFieldUseItemEffect.h"

class Sprite;

class ItemData
{
	friend class DataBase;

private: // é¿ë‘çÏê¨ã÷é~
	ItemData() = default;
	~ItemData() = default;

public:
	enum ID
	{
		PORTION = DataBase::HEAL_ITEM_ID_START,
		MAGIC_PORTION,

		BOMB = DataBase::DAMAGE_ITEM_ID_START,

		SUBDUE_PROOF = DataBase::FIELD_USE_ITEM_ID_START,

		BEGINNERS_SWORD = DataBase::EQUIPMENT_ID_START,
		BEGINNERS_ARMOR,
	};

	enum ItemType { HEAL, DAMAGE, FIELD_USE, EQUIPMENT };

	struct BaseData
	{
		std::wstring name;
		size_t id;
		ItemType type;
		std::shared_ptr<Sprite> icon; // âÊëú
		std::wstring info;
	};

private:
	static std::vector<BaseData> mItems;
	static void LoadItemData();
	static void Initialize();
	static void Release();

public:
	static const BaseData* GetBaseData(const size_t id);
};

//--------------------------------------------------------------
// UseItemData
//--------------------------------------------------------------
class UseItemData
{
public:
	enum Target { PARTY,  ENEMY };
	enum Range  { SINGLE, ALL, GROUP };
	enum Rate   { VALUE,  PERCENT };

	struct Param
	{
		const ItemData::BaseData* base;
		Target target;
		Range range;
		Rate rate;
		int hpValue;
		int mpValue;
		int soundSlot;
		int effectSlot;
	};

public:
	static const Param GetParam(const size_t id);
};

//--------------------------------------------------------------
// FieldUseItemData
//--------------------------------------------------------------
class FieldUseItemData
{
public:
	struct Param
	{
		const ItemData::BaseData* base;
		std::shared_ptr<IFieldUseItemEffect> effect;
	};

public:
	static const Param GetParam(const size_t id);
};

//--------------------------------------------------------------
// EquipmentData
//--------------------------------------------------------------
class EquipmentData
{
public:
	enum Type {WEAPON, ARMOR, TYPE_NUM};

	struct Param
	{
		const ItemData::BaseData* base;
		Type type;
		int atk;
		int def;
		int spd;
		std::vector<bool> equipable;
	};

public:
	static const Param GetParam(const size_t id);
};