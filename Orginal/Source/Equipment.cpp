#include "Equipment.h"

#include "Character.h"
#include "DataBase.h"
#include "EquipmentData.h"
#include "Singleton.h"

bool Equipment::Equip(const Character* equipChara, int id)
{
	const EquipmentData::Param* p = Singleton<DataBase>().GetInstance().GetEquipmentData()->GetParam(id);

	// 装備できるかチェック
	bool equipable = p->equipable[equipChara->GetCharaID() - DataBase::PL_ID_START];
	if (!equipable) return false;


	// まだ装備品がないなら
	if (mEquipments.empty())
	{
		mEquipments.push_back(p);
	}
	else // あれば
	{
		// 同タイプのものがないかチェック
		bool find = false;
		for (auto& param : mEquipments)
		{
			// あれば差し替える
			if (param->type == p->type)
			{
				param = p;
				find = true;
				break;
			}
		}
		
		// もし同タイプのものがない場合は、そのまま装備する
		if (!find)
		{
			mEquipments.push_back(p);
		}
	}

	return true;
}

void Equipment::UnEquip(int id)
{
	// 同じIDのものを探して、ベクターから削除
	for (auto it = mEquipments.begin(); it != mEquipments.end(); ++it)
	{
		if ((*it)->id == id)
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
		if (param->type == type)
		{
			ret = param;
			break;
		}
	}

	return ret;
}
