#pragma once
#include <vector>

class DropData
{
	// ���ԍ쐬�֎~
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

	// @param enemyID : �G��ID
	// @param outDropItemID : �o�́A�h���b�v����A�C�e����ID(-1�Ȃ�h���b�v�Ȃ�)
	static void DecideDropItem(int enemyID, int* outDropItemID);
};