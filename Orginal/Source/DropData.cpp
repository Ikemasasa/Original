#include "DropData.h"

#include "lib/Random.h"

#include "CSVLoader.h"
#include "DataBase.h"

void DropData::GetDropData(const int enemyID, DropEnemyData* outDropEnemy)
{	
	const char* filename = "Data/Database/EnmDrop.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	// データがある行を取得する
	const int LINE_INDEX = enemyID - DataBase::ENM_ID_START;
	std::vector<std::string> chunks;
	loader.GetChunks(allLine[LINE_INDEX], &chunks);


	DropEnemyData ret;
	if (chunks.size() > 0)
	{
		ret.enemyID = enemyID;

		int index = 2;// 敵名、敵IDの情報を飛ばす
		// 以降、アイテム名、アイテムID、ドロップ率（%）で並んでる 
		while (index < chunks.size())
		{
			DropItemData itemData;

			++index; // アイテム名無視
			itemData.itemID = std::stoi(chunks[index++]);
			itemData.dropPercent = std::stof(chunks[index++]);
			ret.dropItemData.push_back(itemData);
		}
		
	}

	loader.Close();

	// 出力データに設定
	*outDropEnemy = ret;
}

void DropData::DecideDropItem(int enemyID, int* outDropItemID)
{
	DropEnemyData dropEnemyData;
	GetDropData(enemyID, &dropEnemyData);

	int dropItemID = -1;
	float rnd = Random::RandomRangef(0.0f, 100.0f);
	for (auto& dropItemData : dropEnemyData.dropItemData)
	{
		rnd -= dropItemData.dropPercent;

		if (rnd < 0.0f)
		{
			dropItemID = dropItemData.itemID;
			break;
		}
	}

	// 出力データに設定
	*outDropItemID = dropItemID;
}
