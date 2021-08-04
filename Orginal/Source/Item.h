#pragma once
#include <vector>

#include "DataBase.h"
#include "ItemData.h"

class Item
{
	std::vector<ItemData::ItemParam> mItems;

public:
	void Add(ItemData::ItemParam param);
	void Sub(const int index);

	int GetItemNum() const { return static_cast<int>(mItems.size()); }
	const ItemData::ItemParam* GetItemParam(const int index) const { return &mItems[index]; }
};