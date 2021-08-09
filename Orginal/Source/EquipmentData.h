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
		std::wstring name;			  // õiÌ¼O
		int id;						  // õiÌID
		std::shared_ptr<Sprite> icon; // õiÌACRæ
		Type type;					  // õiÌ^Cv
		int atk;					  // UÍã¸Ê
		int def;					  // häÍã¸Ê
		int spd;					  // f³ã¸Ê
		std::vector<bool> equipable;  // õÅ«éLXg
	};

private:
	std::vector<Param> mEquipments;

	void Initialize();
public:
	EquipmentData();
	~EquipmentData();

	const Param* GetParam(size_t id) ;
};