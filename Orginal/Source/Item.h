#pragma once
#include <list>
#include <iterator>

#include "DataBase.h"
#include "ItemData.h"

class Item
{
	std::list<ItemData::ItemParam> mItems;

public:
	void Add(ItemData::ItemParam param);
	void Sub(const int itemID);

	int GetItemNum() const { return static_cast<int>(mItems.size()); }
	const ItemData::ItemParam* GetItemParam(const int index) const { return &(*std::next(mItems.begin(), index)); }
};