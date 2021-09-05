#pragma once
#include <list>
#include <iterator>

#include "DataBase.h"
#include "ItemData.h"

class Item
{
	std::list<ItemData::BaseData> mItems;

public:
	void Add(const int itemID);
	void Sub(const int itemID);

	int GetItemNum() const { return static_cast<int>(mItems.size()); }
	const ItemData::BaseData* GetItemParam(const int index) const;
};