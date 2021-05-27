#pragma once
#include <memory>
#include <string>
#include <vector>

#include "DataBase.h"

class Texture;

class ItemData
{
public:

	// item�̃G�N�Z����reference�ƍ��킹�邱��
	enum ID
	{
		PORTION = DataBase::ITEM_ID_START,
		MAGIC_PORTION,
		BOMB,
	};

	enum Effect { HEAL,   DAMAGE, BUFF, DEBUFF };
	enum Target { PARTY,  ENEMY };
	enum Range  { SINGLE, GROUP,  ALL, Rect };
	enum Rate   { VALUE,  PERCENT };

	struct ItemParam
	{
		size_t id;		  // �A�C�e��ID(����H ����)
		std::string name; // �A�C�e����
		std::shared_ptr<Texture> icon; // �摜
		Effect effect;	  // ����
		Target target;	  // �Ώ�
		Range range;	  // ���ʔ͈�	
		Rate rate;		  // ���ʊ���
		int hpValue;      // HP���ʗ�
		int mpValue;	  // MP���ʗ�
		int atkValue;	  // �U�����ʗ�
		int defValue;	  // �h����ʗ�
		std::string info; // ������
	};

private:
	std::vector<ItemParam> mItems;
	size_t GetIndex(size_t id) { return id - (size_t)DataBase::ITEM_ID_START; }

public:
	ItemData();
	~ItemData();

	void LoadItemData();

	const ItemParam& GetItemParam(size_t index) const { return mItems[index]; }
};