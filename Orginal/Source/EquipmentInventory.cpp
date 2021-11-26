#include "EquipmentInventory.h"

#include "ItemData.h"

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
	const EquipmentData::Param param = EquipmentData::GetParam(id);
	// if (param.base->type != ItemData::EQUIPMENT) return;

	// インベントリに追加
	Data data;
	data.equipChara = nullptr;
	data.equipmentID = param.base->id;
	mEquipmentsData[param.type].push_back(data);
}

void EquipmentInventory::Remove(int id)
{
	// パラメータを取得
	const EquipmentData::Param param = EquipmentData::GetParam(id);

	// 同一のIDを検索して見つけたら削除
	for (auto it = mEquipmentsData[param.type].begin(); it != mEquipmentsData[param.type].end(); ++it)
	{
		if (it->equipmentID == id)
		{
			mEquipmentsData[param.type].erase(it);
			break;
		}
	}
}

void EquipmentInventory::UnSetChara(EquipmentData::Type type, const Character* chara)
{
	// type内の同一キャラを検索し、見つけたら外す
	for (auto& equipment : mEquipmentsData[type])
	{
		if (equipment.equipChara == chara)
		{
			equipment.equipChara = nullptr;
			break;
		}
	}
}
