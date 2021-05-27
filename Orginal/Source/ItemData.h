#pragma once
#include <memory>
#include <string>
#include <vector>

#include "DataBase.h"

class Texture;

class ItemData
{
public:

	// itemのエクセルのreferenceと合わせること
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
		size_t id;		  // アイテムID(いる？ いる)
		std::string name; // アイテム名
		std::shared_ptr<Texture> icon; // 画像
		Effect effect;	  // 効果
		Target target;	  // 対象
		Range range;	  // 効果範囲	
		Rate rate;		  // 効果割合
		int hpValue;      // HP効果量
		int mpValue;	  // MP効果量
		int atkValue;	  // 攻撃効果量
		int defValue;	  // 防御効果量
		std::string info; // 説明文
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