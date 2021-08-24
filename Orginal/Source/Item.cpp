#include "Item.h"

void Item::Add(ItemData::ItemParam param)
{
	// �}���ꏊ��T��
	auto insertIterator = mItems.begin();
	for (auto it = mItems.begin(); it != mItems.end(); ++it)
	{
		if (it->id <= param.id) insertIterator = it;
		if (insertIterator->id >= param.id) break;
	}

	// �}��( param.id ��� insert���ID�̕����傫�������疖��
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