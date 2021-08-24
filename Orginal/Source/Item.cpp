#include "Item.h"

void Item::Add(ItemData::ItemParam param)
{
	// ‘}“üêŠ‚ð’T‚·
	auto insertIterator = mItems.begin();
	for (auto it = mItems.begin(); it != mItems.end(); ++it)
	{
		if (it->id <= param.id) insertIterator = it;
		if (insertIterator->id >= param.id) break;
	}

	// ‘}“ü( param.id ‚æ‚è insertæ‚ÌID‚Ì•û‚ª‘å‚«‚©‚Á‚½‚ç––”ö
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