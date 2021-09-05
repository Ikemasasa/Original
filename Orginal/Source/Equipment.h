#pragma once
#include <vector>

#include "ItemData.h"

class Character;

class Equipment
{
	std::vector<EquipmentData::Param> mEquipments;

public:
	bool Equip(const Character* equipChara, int id);
	void UnEquip(int id);
	void UnEquip(EquipmentData::Type type);

	const EquipmentData::Param* GetParam(const EquipmentData::Type type) const;
	size_t GetNum() const { return mEquipments.size(); }
};