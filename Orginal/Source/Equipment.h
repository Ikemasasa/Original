#pragma once
#include <vector>

#include "ItemData.h"

class Character;

/*
	�����N���X
	Status�N���X�ɓ�������Ă�
*/
class Equipment
{
	// �ϐ�
	std::vector<EquipmentData::Param> mEquipments; // �������Ă��镨���X�g

public: // �֐�

	// ����
	bool Equip(const Character* equipChara, int id);

	// �������O��
	void UnEquip(int id);
	void UnEquip(EquipmentData::Type type);

	// �������Ă��镨�̃p�����[�^���擾
	const EquipmentData::Param* GetParam(const EquipmentData::Type type) const;

	// �������Ă�����̂̐����擾
	size_t GetNum() const { return mEquipments.size(); }
};