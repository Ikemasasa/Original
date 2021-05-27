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
		// charaID����ǂ̎��(�G���v���C���[����)�𔻕�
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

	std::string line;  // 1�s�擾�p
	std::getline(fin, line); // �^�O�H������̂�2���s�擾
	std::getline(fin, line);

	// �ϐ��p��
	std::vector<std::string> meshFilenames;
	std::vector<int> motionIDs;
	const char* path = "Data/Mesh/";
	const char delim = ',';
	while (std::getline(fin, line)) // ��s�ǂݍ���
	{
		std::istringstream istr(line);
		std::string chunk; // �R���}��؂�̓��e�擾�p

		// ���O(char)
		std::getline(istr, chunk, delim); // ��s�ǂݍ��񂾂̂��R���}��؂�Ŏ擾

		// charaID(int)
		std::getline(istr, chunk, delim);
		int id = std::stoi(chunk);
		if (charaID == id)
		{
			md.charaID = id;

			// ���[�V�����̎��(�g���) 
			std::getline(istr, chunk, delim);

			// ���[�V������ID
			std::getline(istr, chunk, delim);
			motionIDs.emplace_back(std::stoi(chunk));

			// ���[�V�����̃t�@�C����
			std::getline(istr, chunk, delim);
			meshFilenames.emplace_back(path + chunk);
		}
		if (charaID < id) break;
	}
	fin.close();


	// ���b�V���ǂ݂���
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
