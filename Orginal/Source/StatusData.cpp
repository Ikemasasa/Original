#include "StatusData.h"

#include <fstream>
#include <sstream>

#include "DataBase.h"

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

	std::string line;  // 1行取得用
	std::getline(fin, line); // タグ？があるので一回一行取得

	const char delim = ',';
	while (std::getline(fin, line)) // 一行読み込み
	{
		std::istringstream istr(line);
		std::string chunk; // コンマ区切りの内容取得用
		Status s = {};

		// 名前(char)
		std::getline(istr, chunk, delim); // 一行読み込んだのをコンマ区切りで取得
		s.name = chunk;

		// id(int)
		std::getline(istr, chunk, delim);
		s.id = std::stoi(chunk);

		// HP(int)
		std::getline(istr, chunk, delim);
		s.hp = std::stoi(chunk);

		// MP(int)
		std::getline(istr, chunk, delim);
		s.mp = std::stoi(chunk);

		// STR(int)
		std::getline(istr, chunk, delim);
		s.str = std::stoi(chunk);

		// VIT(int)
		std::getline(istr, chunk, delim);
		s.vit = std::stoi(chunk);

		// AGI(int)
		std::getline(istr, chunk, delim);
		s.agi = std::stoi(chunk);

		mPLStatus.emplace_back(s);
	}

	fin.close();
}

void StatusData::LoadEnmStatus()
{
	const char* filename = "Data/DataBase/EnmStatus.csv";

	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) return;

	std::string line;  // 1行取得用
	std::getline(fin, line); // タグ？があるので一回一行取得

	const char delim = ',';
	while (std::getline(fin, line)) // 一行読み込み
	{
		std::istringstream istr(line);
		std::string chunk; // コンマ区切りの内容取得用
		Status s = {};

		// 名前(char)
		std::getline(istr, chunk, delim); // 一行読み込んだのをコンマ区切りで取得
		s.name = chunk;

		// id(int)
		std::getline(istr, chunk, delim);
		s.id = std::stoi(chunk);

		// HP(int)
		std::getline(istr, chunk, delim);
		s.hp = std::stoi(chunk);

		// MP(int)
		std::getline(istr, chunk, delim);
		s.mp = std::stoi(chunk);

		// STR(int)
		std::getline(istr, chunk, delim);
		s.str = std::stoi(chunk);

		// VIT(int)
		std::getline(istr, chunk, delim);
		s.vit = std::stoi(chunk);

		// AGI(int)
		std::getline(istr, chunk, delim);
		s.agi = std::stoi(chunk);

		mEnmStatus.emplace_back(s);
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
