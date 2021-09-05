#pragma once
#include <memory>

class DataBase
{
private:
	DataBase() = default;
	~DataBase() = default;

public:
	static const int PL_ID_START			 = 10000;
	static const int NPC_ID_START			 = 15000;
	static const int ENM_ID_START			 = 20000;
	static const int TERRAIN_ID_START		 = 30000;
	static const int HEAL_ITEM_ID_START		 = 40000;
	static const int DAMAGE_ITEM_ID_START	 = 45000;
	static const int FIELD_USE_ITEM_ID_START = 50000;
	static const int EQUIPMENT_ID_START		 = 60000;

public:
	static void Initialize();
	static void Release();
};