#include "Equipment.h"

#include "Character.h"
#include "DataBase.h"
#include "EquipmentData.h"
#include "Singleton.h"

bool Equipment::Equip(const Character* equipChara, int id)
{
	const EquipmentData::Param* p = Singleton<DataBase>().GetInstance().GetEquipmentData()->GetParam(id);

	// �����ł��邩�`�F�b�N
	bool equipable = p->equipable[equipChara->GetCharaID() - DataBase::PL_ID_START];
	if (!equipable) return false;


	// �܂������i���Ȃ��Ȃ�
	if (mEquipments.empty())
	{
		mEquipments.push_back(p);
	}
	else // �����
	{
		// ���^�C�v�̂��̂��Ȃ����`�F�b�N
		bool find = false;
		for (auto& param : mEquipments)
		{
			// ����΍����ւ���
			if (param->type == p->type)
			{
				param = p;
				find = true;
				break;
			}
		}
		
		// �������^�C�v�̂��̂��Ȃ��ꍇ�́A���̂܂ܑ�������
		if (!find)
		{
			mEquipments.push_back(p);
		}
	}

	return true;
}

void Equipment::UnEquip(int id)
{
	// ����ID�̂��̂�T���āA�x�N�^�[����폜
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
