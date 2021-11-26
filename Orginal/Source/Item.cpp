#include "Item.h"

void ItemInventory::Add(const int itemID)
{
	ItemData::BaseData param = *ItemData::GetBaseData(itemID);
	if (param.type == ItemData::ItemType::EQUIPMENT) return; // 装備品は別インベントリ

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

void ItemInventory::Sub(const int itemID)
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

const ItemData::BaseData* ItemInventory::GetItemParam(const int index) const
{
	// リストがからならnull
	if (mItems.empty()) return nullptr;

	return &(*std::next(mItems.begin(), index));
}
