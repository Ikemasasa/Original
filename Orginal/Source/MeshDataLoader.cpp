#include "MeshDataLoader.h"

#include "lib/Math.h"

#include "Character.h"
#include "CSVLoader.h"
#include "DataBase.h"

MeshDataLoader::MeshData MeshDataLoader::LoadMesh(int charaID)
{
	MeshData md;

	const char* csvFilename = nullptr;
	const char* fbxFilepath = "Data/Mesh/";
	{
		// charaID����ǂ̎��(�G���v���C���[����)�𔻕�
		int pl = abs(charaID - DataBase::PL_ID_START);
		int npc = abs(charaID - DataBase::NPC_ID_START);
		int enm = abs(charaID - DataBase::ENM_ID_START);
		int ter = abs(charaID - DataBase::TERRAIN_ID_START);
		int check = Math::Min(npc, Math::Min(Math::Min(pl, enm), ter));

		if (pl == check)
		{
			csvFilename = "Data/DataBase/PLMesh.csv";
			fbxFilepath = "Data/Mesh/Players/";
		}
		else if (npc == check)
		{
			csvFilename = "Data/DataBase/NPCMesh.csv";
			fbxFilepath = "Data/Mesh/NPCs/";
		}
		else if (enm == check)
		{
			csvFilename = "Data/DataBase/EnmMesh.csv";
		}
		else if (ter == check)
		{
			csvFilename = "Data/DataBase/TerrainMesh.csv";
		}
	}

	CSVLoader loader;
	loader.Open(csvFilename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);
	
	// ���[�V����ID�A�t�@�C������ۑ�����ϐ���錾
	std::vector<int> motionIDs;
	std::vector<std::string> filenames;
	for (const auto& line : allLine)
	{
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		const int ID_INDEX = 1;
		int id = std::stoi(chunks[ID_INDEX]);
		if (id == charaID)
		{
			int index = 2; // �L�������A�L����ID�𖳎�

			// ���[�V����ID�A�t�@�C�����ۑ�
			motionIDs.emplace_back(std::stoi(chunks[index++]));
			filenames.emplace_back(fbxFilepath + chunks[index++]);
		}
		if (id > charaID) break; // charaID���傫���Ȃ�����break;
	}
	loader.Close();

	// ���b�V���ǂ݂���
	{
		if (filenames.empty()) return md;
		if (motionIDs[0] != Character::DEFAULT) return md;

		std::shared_ptr<SkinnedMesh> mesh(std::make_shared<SkinnedMesh>(filenames[0].c_str()) );
		size_t fileNum = filenames.size();
		for (size_t i = 1; i < fileNum; ++i)
		{
			if (motionIDs[i] >= Character::MOTION_MAX) break;
			mesh->AddMotion(filenames[i].c_str(), motionIDs[i]);
		}

		md.mesh = mesh;
	}

	return md;
}
