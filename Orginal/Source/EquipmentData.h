#pragma once
#include <memory>
#include <string>
#include <vector>

#include "DataBase.h"
#include "PlayerManager.h"

class Sprite;

class EquipmentData
{
public:
	enum ID
	{
		BEGINNERS_SWORD = DataBase::EQUIPMENT_ID_START,
		BEGINNERS_ARMOR,

		NUM
	};

	struct Param
	{
		int id;
		std::string name;
		std::shared_ptr<Sprite> icon;
		int atk;
		int def;
		int spd;
		std::vector<bool> equipable;
	};

private:
	std::vector<Param> mEquipments;

	void Initialize();
public:
	EquipmentData();
	~EquipmentData();

	Param GetParam(size_t id);
};