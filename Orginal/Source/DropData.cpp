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

	std::string line;  // 1�s�擾�p
	std::getline(fin, line); // �^�O��
	// �f�[�^������s�܂Ŕ�΂�
	for (int i = DataBase::ENM_ID_START; i <= enemyID; ++i)
	{
		std::getline(fin, line);
	}

	std::istringstream istr(line);
	std::string chunk; // �R���}��؂�̓��e�擾�p
	const char delim = ','; // ��؂蕶��
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
		index += 2; // �G���A�GID, �A�C�e�����̏����΂�

		// �ȍ~�A�A�C�e�����A�A�C�e��ID�A�h���b�v���i%�j�ŕ���ł� 
		while (!data[index].empty())
		{
			DropItemData itemData;

			++index; // �A�C�e��������
			itemData.itemID = std::stoi(data[index++]);
			itemData.dropPercent = std::stof(data[index++]);
			ret.dropItemData.push_back(itemData);
		}
		
	}

	fin.close();

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
