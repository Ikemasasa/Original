#pragma once
#include <vector>

class DropData
{
	struct DropItemData
	{
		int itemID = -1;
		float dropPercent = 0;
	};

	struct DropEnemyData
	{
		int enemyID = -1;
		std::vector<DropItemData> dropItemData;
	};
	
	void GetDropData(const int enemyID, DropEnemyData* outDropEnemy);
public:
	DropData() = default;
	~DropData() = default;

	// @param enemyID : 敵のID
	// @param outDropItemID : 出力、ドロップするアイテムのID(-1ならドロップなし)
	void DecideDropItem(int enemyID, int* outDropItemID);
};