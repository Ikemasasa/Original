#pragma once
#include <memory>

class StatusData;
class ItemData;

// Singleton‚ð’Ê‚·
class DataBase
{
public:
	static const int PL_ID_START	  = 10000;
	static const int ENM_ID_START	  = 20000;
	static const int TERRAIN_ID_START = 30000;
	static const int ITEM_ID_START    = 40000;

private:
	std::unique_ptr<StatusData> mStatusData;
	std::unique_ptr<ItemData> mItemData;

public:
	DataBase();
	~DataBase();

	StatusData* GetStatusData() { return mStatusData.get(); }
	ItemData* GetItemData() { return mItemData.get(); }
};