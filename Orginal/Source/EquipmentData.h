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
		std::wstring name;			  // �����i�̖��O
		int id;						  // �����i��ID
		std::shared_ptr<Sprite> icon; // �����i�̃A�C�R���摜
		Type type;					  // �����i�̃^�C�v
		int atk;					  // �U���͏㏸��
		int def;					  // �h��͏㏸��
		int spd;					  // �f�����㏸��
		std::vector<bool> equipable;  // �����ł���L�������X�g
	};

private:
	std::vector<Param> mEquipments;

	void Initialize();
public:
	EquipmentData();
	~EquipmentData();

	const Param* GetParam(size_t id) ;
};