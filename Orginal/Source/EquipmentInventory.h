#pragma once
#include "EquipmentData.h"

class Character;

class EquipmentInventory
{
public:
	struct Data
	{
		const Character* equipmentChara = nullptr;
		int equipmentID = -1;
	};

private:

	std::vector<Data> mEquipmentsData[EquipmentData::TYPE_MAX];

public:
	EquipmentInventory();
	~EquipmentInventory();

	void Push(int id);
	void Remove(int id);

	const std::vector<Data>& GetInventory(EquipmentData::Type type) const { return mEquipmentsData[type]; }
	void SetChara(EquipmentData::Type type, const Character* chara, int inventoryIndex) { mEquipmentsData[type][inventoryIndex].equipmentChara = chara; }
	void UnSetChara(EquipmentData::Type type, int inventoryIndex) { mEquipmentsData[type][inventoryIndex].equipmentChara = nullptr; }
};