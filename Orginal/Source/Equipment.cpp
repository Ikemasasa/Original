#include "Equipment.h"

#include "Character.h"

bool Equipment::Equip(const Character* equipChara, int id)
{
	const EquipmentData::Param param = EquipmentData::GetParam(id);

	// �����ł��邩�`�F�b�N
	bool equipable = param.equipable[equipChara->GetCharaID() - DataBase::PL_ID_START];
	if (!equipable) return false;


	// �܂������i���Ȃ��Ȃ瑕������
	if (mEquipments.empty())
	{
		mEquipments.push_back(param);
	}
	else 
	{
		// �����i������΁A���^�C�v�̂��̂��Ȃ����`�F�b�N
		bool find = false;
		for (auto& equipment : mEquipments)
		{
			// ����΍����ւ���
			if (equipment.type == param.type)
			{
				equipment = param;
				find = true;
				break;
			}
		}

		// �������^�C�v�̂��̂��Ȃ��ꍇ�́A���̂܂ܑ�������
		if (!find)
		{
			mEquipments.push_back(param);
		}
	}

	return true;
}

void Equipment::UnEquip(int id)
{
	// ����ID�̂��̂�T���āA�x�N�^�[����폜
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
	// ����ID�̂��̂�T���āA�x�N�^�[����폜
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
