#include "EquipmentInventory.h"

#include "DataBase.h"
#include "EquipmentData.h"
#include "Singleton.h"

EquipmentInventory::EquipmentInventory()
{
	for (auto& equipment : mEquipmentsData)
		equipment.clear();
}

EquipmentInventory::~EquipmentInventory()
{
	for (auto& equipment : mEquipmentsData)
		equipment.clear();
}

void EquipmentInventory::Push(int id)
{
	// パラメーター取得
	const EquipmentData::Param* param = Singleton<DataBase>().GetInstance().GetEquipmentData()->GetParam(id);

	Data data;
	data.equipmentChara = nullptr;
	data.equipmentID = param->id;
	mEquipmentsData[param->type].push_back(data);
}

void EquipmentInventory::Remove(int id)
{
	// type取得
	const EquipmentData::Param* param = Singleton<DataBase>().GetInstance().GetEquipmentData()->GetParam(id);

	for (auto it = mEquipmentsData[param->type].begin(); it != mEquipmentsData[param->type].end(); ++it)
	{
		if (it->equipmentID == id)
		{
			mEquipmentsData[param->type].erase(it);
			break;
		}
	}
}