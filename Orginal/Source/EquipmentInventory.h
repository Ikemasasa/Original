#pragma once
#include "ItemData.h"

class Character;

/*
	�����i�̃C���x���g���N���X
	����A�C�e���Ƃ��̃C���x���g���ƕ�����������
*/
class EquipmentInventory
{
public: // �ϐ�
	struct Data
	{
		const Character* equipChara = nullptr; // �������Ă���L����
		int equipmentID = -1;				   // �����i��ID
	};

private:
	std::vector<Data> mEquipmentsData[EquipmentData::TYPE_NUM]; // �^�C�v���Ƃ̑����i�̃��X�g

public: // �֐�

	// �R���X�g���N�^
	EquipmentInventory();

	// �f�X�g���N�^
	~EquipmentInventory();

	// �C���x���g���ɒǉ�
	void Push(int id);

	// �C���x���g������폜
	void Remove(int id);

	// �C���x���g�����擾
	const std::vector<Data>& GetInventory(EquipmentData::Type type) const { return mEquipmentsData[type]; }

	// �����L�����ɓo�^
	void SetChara(EquipmentData::Type type, const Character* chara, int inventoryIndex) { mEquipmentsData[type][inventoryIndex].equipChara = chara; }

	// �����L�������疕��
	void UnSetChara(EquipmentData::Type type, const Character* chara);
};