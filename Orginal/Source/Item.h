#pragma once
#include <vector>

#include "DataBase.h"
#include "ItemData.h"

class Item
{
	// std::map<int, int> mItems; // key:ID, value:êî
	std::vector<ItemData::ItemParam> mItems;

public:
	void Add(ItemData::ItemParam param);
	void Sub(const int index);

	const std::vector<ItemData::ItemParam>& GetAll() { return mItems; }
};