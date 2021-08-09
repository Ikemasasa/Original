#pragma once
#include <memory>
#include <string>
#include <vector>

#include "DataBase.h"

class Sprite;

class EquipmentData
{
public:
	enum ID
	{
		BEGINNERS_SWORD = DataBase::EQUIPMENT_ID_START,
		BEGINNERS_ARMOR
	};

	enum Type { WEAPON, ARMOR, TYPE_MAX};

	struct Param
	{
		std::wstring name;			  // 装備品の名前
		int id;						  // 装備品のID
		std::shared_ptr<Sprite> icon; // 装備品のアイコン画像
		Type type;					  // 装備品のタイプ
		int atk;					  // 攻撃力上昇量
		int def;					  // 防御力上昇量
		int spd;					  // 素早さ上昇量
		std::vector<bool> equipable;  // 装備できるキャラリスト
	};

private:
	std::vector<Param> mEquipments;

	void Initialize();
public:
	EquipmentData();
	~EquipmentData();

	const Param* GetParam(size_t id) ;
};