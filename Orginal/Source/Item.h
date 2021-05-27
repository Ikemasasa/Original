#pragma once
#include <map>

#include "DataBase.h"

class Item
{
	std::map<int, int> mItems; // key:ID, value:êî

	int GetIndex(const int id) { return id - DataBase::ITEM_ID_START; }
public:
	void Add(const int itemID) { ++mItems[GetIndex(itemID)]; }
	void Sub(const int itemID);

	const std::map<int, int>& GetAll() { return mItems; }
};