#include "MeshDataLoader.h"

#include <fstream>
#include <sstream>
#include <string>

#include "DataBase.h"
#include "lib/Math.h"

MeshDataLoader::MeshData MeshDataLoader::LoadMesh(int charaID)
{
	MeshData md;

	const char* filename = nullptr;
	{
		// charaIDからどの種類(敵かプレイヤーか等)を判別
		int pl = abs(charaID - DataBase::PL_ID_START);
		int enm = abs(charaID - DataBase::ENM_ID_START);
		int ter = abs(charaID - DataBase::TERRAIN_ID_START);
		int check = Math::Min(Math::Min(pl, enm), ter);

		if (pl == check)
		{
			filename = "Data/DataBase/PLMesh.csv";
		}
		else if (enm == check)
		{
			filename = "Data/DataBase/EnmMesh.csv";
		}
		else if (ter == check)
		{
			filename = "Data/DataBase/TerrainMesh.csv";
		}


	}

	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) return md;

	std::string line;  // 1行取得用
	std::getline(fin, line); // タグ？があるので2回一行取得
	std::getline(fin, line);

	// 変数用意
	std::vector<std::string> meshFilenames;
	std::vector<int> motionIDs;
	const char* path = "Data/Mesh/";
	const char delim = ',';
	while (std::getline(fin, line)) // 一行読み込み
	{
		std::istringstream istr(line);
		std::string chunk; // コンマ区切りの内容取得用

		// 名前(char)
		std::getline(istr, chunk, delim); // 一行読み込んだのをコンマ区切りで取得

		// charaID(int)
		std::getline(istr, chunk, delim);
		int id = std::stoi(chunk);
		if (charaID == id)
		{
			md.charaID = id;

			// モーションの種類(使わん) 
			std::getline(istr, chunk, delim);

			// モーションのID
			std::getline(istr, chunk, delim);
			motionIDs.emplace_back(std::stoi(chunk));

			// モーションのファイル名
			std::getline(istr, chunk, delim);
			meshFilenames.emplace_back(path + chunk);
		}
		if (charaID < id) break;
	}
	fin.close();


	// メッシュ読みこみ
	{
		if (meshFilenames.empty()) return md;
		if (motionIDs[0] != SkinnedMesh::DEFAULT) return md;

		size_t count = meshFilenames.size();
		std::shared_ptr<SkinnedMesh> mesh(std::make_shared<SkinnedMesh>(meshFilenames[0].c_str()) );
		for (size_t i = 1; i < count; ++i)
		{
			if (motionIDs[i] >= SkinnedMesh::MAX) break;
			mesh->AddMotion(meshFilenames[i].c_str(), (SkinnedMesh::MotionType)motionIDs[i]);
		}

		md.mesh = mesh;
	}

	return md;
}
