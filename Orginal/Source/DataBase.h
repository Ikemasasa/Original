#pragma once
#include <memory>

class DropData;
class EquipmentData;
class ItemData;
class StatusData;
class SkillData;

// Singleton‚ð’Ê‚·
class DataBase
{
public:
	static const int PL_ID_START	  = 10000;
	static const int ENM_ID_START	  = 20000;
	static const int TERRAIN_ID_START = 30000;
	static const int ITEM_ID_START    = 40000;
	static const int EQUIPMENT_ID_START = 50000;

private:
	std::unique_ptr<StatusData>	   mStatusData;
	std::unique_ptr<ItemData>	   mItemData;
	std::unique_ptr<EquipmentData> mEquipmentData;
	std::unique_ptr<DropData> mDropData;
	std::unique_ptr<SkillData> mSkillData;

public:
	DataBase();
	~DataBase();

	DropData* GetDropData() { return mDropData.get(); }
	EquipmentData* GetEquipmentData() { return mEquipmentData.get(); }
	ItemData* GetItemData() { return mItemData.get(); }
	StatusData* GetStatusData() { return mStatusData.get(); }
	SkillData* GetSkillData() { return mSkillData.get(); }
};