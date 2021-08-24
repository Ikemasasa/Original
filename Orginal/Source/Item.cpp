#include "Item.h"

void Item::Add(ItemData::ItemParam param)
{
	// 挿入場所を探す
	auto insertIterator = mItems.begin();
	for (auto it = mItems.begin(); it != mItems.end(); ++it)
	{
		if (it->id <= param.id) insertIterator = it;
		if (insertIterator->id >= param.id) break;
	}

	// 挿入( param.id より insert先のIDの方が大きかったら末尾
	if (!mItems.empty() && insertIterator->id >= param.id) mItems.insert(insertIterator, param);
	else mItems.push_back(param);

}

void Item::Sub(const int itemID)
{
	for (auto it = mItems.begin(); it != mItems.end(); ++it)
	{
		if (it->id == itemID)
		{
			mItems.erase(it);
			break;
		}
	}
}