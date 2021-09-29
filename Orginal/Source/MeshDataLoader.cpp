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
		// charaIDからどの種類(敵かプレイヤーか等)を判別
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
	
	// モーションID、ファイル名を保存する変数を宣言
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
			int index = 2; // キャラ名、キャラIDを無視

			// モーションID、ファイル名保存
			motionIDs.emplace_back(std::stoi(chunks[index++]));
			filenames.emplace_back(fbxFilepath + chunks[index++]);
		}
		if (id > charaID) break; // charaIDより大きくなったらbreak;
	}
	loader.Close();

	// メッシュ読みこみ
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
