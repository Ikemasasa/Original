#include "DropData.h"

#include <fstream>
#include <sstream>

#include "lib/Random.h"

#include "DataBase.h"

void DropData::GetDropData(const int enemyID, DropEnemyData* outDropEnemy)
{	
	const char* filename = "Data/Database/DropData.csv";
	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) return;

	std::string line;  // 1行取得用
	std::getline(fin, line); // タグ名
	// データがある行まで飛ばす
	for (int i = DataBase::ENM_ID_START; i <= enemyID; ++i)
	{
		std::getline(fin, line);
	}

	std::istringstream istr(line);
	std::string chunk; // コンマ区切りの内容取得用
	const char delim = ','; // 区切り文字
	std::vector<std::string> data;
	while (std::getline(istr, chunk, delim))
	{
		data.push_back(chunk);
	}

	DropEnemyData ret;
	if (!data.empty())
	{
		ret.enemyID = enemyID;

		int index = 0;
		index += 2; // 敵名、敵ID, アイテム名の情報を飛ばす

		// 以降、アイテム名、アイテムID、ドロップ率（%）で並んでる 
		while (!data[index].empty())
		{
			DropItemData itemData;

			++index; // アイテム名無視
			itemData.itemID = std::stoi(data[index++]);
			itemData.dropPercent = std::stof(data[index++]);
			ret.dropItemData.push_back(itemData);
		}
		
	}

	fin.close();

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
