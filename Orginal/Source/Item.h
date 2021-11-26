#pragma once
#include <list>
#include <iterator>

#include "DataBase.h"
#include "ItemData.h"

/*
	����A�C�e���̃C���x���g���N���X
*/
class ItemInventory
{
	// �ϐ�
	std::list<ItemData::BaseData> mItems; // �����A�C�e���̃��X�g

public: // �֐�

	// �C���x���g���ɒǉ�
	void Add(const int itemID);

	// �C���x���g������폜
	void Sub(const int itemID);

	// �C���x���g���̃A�C�e���̐����擾
	int GetItemNum() const { return static_cast<int>(mItems.size()); } 

	// �C���x���g������A�C�e���̃p�����[�^�擾
	const ItemData::BaseData* GetItemParam(const int index) const;
};