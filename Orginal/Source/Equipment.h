#pragma once
#include <vector>

#include "EquipmentData.h"

class Character;

class Equipment
{
	std::vector<const EquipmentData::Param*> mEquipments;

public:
	bool Equip(const Character* equipChara, int id);
	void UnEquip(int id);

	const EquipmentData::Param* GetParam(const EquipmentData::Type type) const;
	size_t GetNum() const { return mEquipments.size(); }
};