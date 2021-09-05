#include "Equipment.h"

#include "Character.h"

bool Equipment::Equip(const Character* equipChara, int id)
{
	const EquipmentData::Param param = EquipmentData::GetParam(id);

	// 装備できるかチェック
	bool equipable = param.equipable[equipChara->GetCharaID() - DataBase::PL_ID_START];
	if (!equipable) return false;


	// まだ装備品がないなら装備する
	if (mEquipments.empty())
	{
		mEquipments.push_back(param);
	}
	else 
	{
		// 装備品があれば、同タイプのものがないかチェック
		bool find = false;
		for (auto& equipment : mEquipments)
		{
			// あれば差し替える
			if (equipment.type == param.type)
			{
				equipment = param;
				find = true;
				break;
			}
		}

		// もし同タイプのものがない場合は、そのまま装備する
		if (!find)
		{
			mEquipments.push_back(param);
		}
	}

	return true;
}

void Equipment::UnEquip(int id)
{
	// 同じIDのものを探して、ベクターから削除
	for (auto it = mEquipments.begin(); it != mEquipments.end(); ++it)
	{
		if ((*it).base->id == id)
		{
			mEquipments.erase(it);
			break;
		}
	}
}


void Equipment::UnEquip(EquipmentData::Type type)
{
	// 同じIDのものを探して、ベクターから削除
	for (auto it = mEquipments.begin(); it != mEquipments.end(); ++it)
	{
		if (it->base->type == type)
		{
			mEquipments.erase(it);
			break;
		}
	}
}

const EquipmentData::Param* Equipment::GetParam(const EquipmentData::Type type) const
{
	const EquipmentData::Param* ret = nullptr;
	for (auto& param : mEquipments)
	{
		if (param.type == type)
		{
			ret = &param;
			break;
		}
	}

	return ret;
}
