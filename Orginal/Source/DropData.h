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

	// @param enemyID : �G��ID
	// @param outDropItemID : �o�́A�h���b�v����A�C�e����ID(-1�Ȃ�h���b�v�Ȃ�)
	void DecideDropItem(int enemyID, int* outDropItemID);
};