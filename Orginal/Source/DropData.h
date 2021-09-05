#pragma once
#include <vector>

class DropData
{
	// 実態作成禁止
	DropData() = default;
	~DropData() = default;

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
	
	static void GetDropData(const int enemyID, DropEnemyData* outDropEnemy);
public:

	// @param enemyID : 敵のID
	// @param outDropItemID : 出力、ドロップするアイテムのID(-1ならドロップなし)
	static void DecideDropItem(int enemyID, int* outDropItemID);
};