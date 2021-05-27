#include "Item.h"

void Item::Sub(const int itemID)
{
	int index = GetIndex(itemID);
	if (mItems.find(index) == mItems.end()) return;

	--mItems[index];
	if (mItems[index] <= 0)
	{
		mItems.erase(index);
	}
}