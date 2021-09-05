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

	// �f�[�^������s���擾����
	const int LINE_INDEX = enemyID - DataBase::ENM_ID_START;
	std::vector<std::string> chunks;
	loader.GetChunks(allLine[LINE_INDEX], &chunks);


	DropEnemyData ret;
	if (chunks.size() > 0)
	{
		ret.enemyID = enemyID;

		int index = 2;// �G���A�GID�̏����΂�
		// �ȍ~�A�A�C�e�����A�A�C�e��ID�A�h���b�v���i%�j�ŕ���ł� 
		while (index < chunks.size())
		{
			DropItemData itemData;

			++index; // �A�C�e��������
			itemData.itemID = std::stoi(chunks[index++]);
			itemData.dropPercent = std::stof(chunks[index++]);
			ret.dropItemData.push_back(itemData);
		}
		
	}

	loader.Close();

	// �o�̓f�[�^�ɐݒ�
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

	// �o�̓f�[�^�ɐݒ�
	*outDropItemID = dropItemID;
}
