#include "StatusData.h"

#include <fstream>
#include <sstream>

#include "DataBase.h"
#include "lib/ConvertString.h"

StatusData::StatusData()
{
	mPLStatus.clear();
	mEnmStatus.clear();

	LoadPLStatus();
	LoadEnmStatus();
}

StatusData::~StatusData()
{
	mPLStatus.clear();
	mEnmStatus.clear();
}

void StatusData::LoadPLStatus()
{
	const char* filename = "Data/DataBase/PLStatus.csv";

	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) return;

	std::string line;  // 1�s�擾�p
	const char delim = ','; // ��؂蕶��
	while (std::getline(fin, line)) // ��s�ǂݍ���
	{
		std::istringstream istr(line);
		std::string chunk; // �R���}��؂�̓��e�擾�p

		std::vector<std::string> data;
		while (std::getline(istr, chunk, delim)) // ��؂育�ƂɎ擾
		{
			if (chunk.empty()) continue;   // �󔒂Ȃ� continue
			if (chunk[0] == '#') continue; // �ŏ��̕�����#�Ȃ�continue

			data.emplace_back(chunk);
		}

		if (data.size() > 0)
		{
			Status s = {};
			int index = 0;
			s.name = ConvertString::ConvertToWstirng(data[index++]);
			s.id = std::stoi(data[index++]);
			s.hp = std::stoi(data[index++]);
			s.maxHP = s.hp;
			s.mp = std::stoi(data[index++]);
			s.maxMP = s.mp;
			s.str = std::stoi(data[index++]);
			s.vit = std::stoi(data[index++]);
			s.agi = std::stoi(data[index++]);

			mPLStatus.emplace_back(s);
		}
	}

	fin.close();
}

void StatusData::LoadEnmStatus()
{
	const char* filename = "Data/DataBase/EnmStatus.csv";

	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) return;

	std::string line;  // 1�s�擾�p
	const char delim = ','; // ��؂蕶��
	while (std::getline(fin, line)) // ��s�ǂݍ���
	{
		std::istringstream istr(line);
		std::string chunk; // �R���}��؂�̓��e�擾�p

		std::vector<std::string> data; // ��؂�̕ۑ��p
		while (std::getline(istr, chunk, delim)) // ��؂育�ƂɎ擾
		{
			if (chunk.empty()) continue;   // �󔒂Ȃ� continue
			if (chunk[0] == '#') continue; // �ŏ��̕�����#�Ȃ�continue

			data.emplace_back(chunk);
		}

		if (data.size() > 0)
		{
			Status s = {};
			int index = 0;
			s.name = ConvertString::ConvertToWstirng(data[index++]);
			s.id = std::stoi(data[index++]);
			s.hp = std::stoi(data[index++]);
			s.maxHP = s.hp;
			s.mp = std::stoi(data[index++]);
			s.maxMP = s.mp;
			s.str = std::stoi(data[index++]);
			s.vit = std::stoi(data[index++]);
			s.agi = std::stoi(data[index++]);

			mEnmStatus.emplace_back(s);
		}
	}

	fin.close();
}


void StatusData::Initialize()
{
	LoadPLStatus();
	LoadEnmStatus();
}

Status StatusData::GetPLStatus(size_t id) const
{
	return mPLStatus[id - DataBase::PL_ID_START];
}

Status StatusData::GetEnmStatus(size_t id) const
{
	return mEnmStatus[id - DataBase::ENM_ID_START];
}

void StatusData::SetPLStatus(size_t charaID, const Status& status)
{
	mPLStatus[charaID - DataBase::PL_ID_START] = status;
}

//void StatusData::SetPLStatus(const std::wstring& name, const Status& status)
//{
//	size_t num = mPLStatus.size();
//	for (size_t i = 0; i < num; ++i)
//	{
//		if (wcscmp(mPLStatus[i].name.c_str(), name.c_str()) != 0) continue;
//		
//		mPLStatus[i] = status;
//	}
//}
