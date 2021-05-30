#include "Item.h"

void Item::Add(ItemData::ItemParam param)
{
	mItems.emplace_back(param);
}

void Item::Sub(const int index)
{
	int count = 0;
	std::vector<ItemData::ItemParam>::iterator it = mItems.begin();
	for (;it != mItems.end(); ++it, ++count)
		if (count == index) break;

	mItems.erase(it);
}