#include "StatusData.h"

#include <fstream>
#include <sstream>

#include "DataBase.h"
#include "lib/ConvertString.h"

//---------------------------------------------
// Status
//---------------------------------------------
int Status::GetAtk() const
{
	int atk = 0;
	const EquipmentData::Param* param = equipments.GetParam(EquipmentData::WEAPON);
	if (param) atk = param->atk;
	atk += str * buffStr.rate;

	return 	atk;
}

int Status::GetDef() const
{
	int def = 0;
	const EquipmentData::Param* param = equipments.GetParam(EquipmentData::ARMOR);
	if (param) def = param->def;
	def += vit * buffVit.rate;

	return 	def;
}

int Status::GetSpd() const
{
	// 今はagiを変えすだけ
	return agi;
}

void Status::SetBuffStrRate(const float rate, const int turn)
{
	buffStr.rate = rate;
	buffStr.turn = turn;
}

void Status::SetBuffVitRate(const float rate, const int turn)
{
	buffVit.rate = rate;
	buffVit.turn = turn;
}

void Status::AdvanceBuffTurn()
{
	// 攻撃バフ
	buffStr.turn = Math::Max(0, buffStr.turn - 1);
	if (buffStr.turn <= 0) buffStr.rate = 1.0f;

	// 防御バフ
	buffVit.turn = Math::Max(0, buffVit.turn - 1);
	if (buffVit.turn <= 0) buffVit.rate = 1.0f;
}

//---------------------------------------------
// StatusData
//---------------------------------------------
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
	const char delim = ','; // 区切り文字
	while (std::getline(fin, line)) // 一行読み込み
	{
		std::istringstream istr(line);
		std::string chunk; // コンマ区切りの内容取得用

		std::vector<std::string> data;
		while (std::getline(istr, chunk, delim)) // 区切りごとに取得
		{
			if (chunk.empty()) continue;   // 空白なら continue
			if (chunk[0] == '#') continue; // 最初の文字が#ならcontinue

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

	std::string line;  // 1行取得用
	const char delim = ','; // 区切り文字
	while (std::getline(fin, line)) // 一行読み込み
	{
		std::istringstream istr(line);
		std::string chunk; // コンマ区切りの内容取得用

		std::vector<std::string> data; // 区切りの保存用
		while (std::getline(istr, chunk, delim)) // 区切りごとに取得
		{
			if (chunk.empty()) continue;   // 空白なら continue
			if (chunk[0] == '#') continue; // 最初の文字が#ならcontinue

			data.emplace_back(chunk);
		}

		if (data.size() > 0)
		{
			Status s = {};
			int index = 0;
			s.name = ConvertString::ConvertToWstirng(data[index++]);
			s.id = std::stoi(data[index++]);

			// 敵タイプ、タイプIDを無視
			index += 2;

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

StatusData::EnemyType StatusData::GetEnmType(size_t id) const
{
	const char* filename = "Data/DataBase/EnmStatus.csv";

	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) return NONE;

	std::string line;  // 1行取得用

	// タグを無視
	std::getline(fin, line);

	for (int i = DataBase::ENM_ID_START; i <= id; ++i)
	{
		std::getline(fin, line);
	}

	const char delim = ','; // 区切り文字
	std::istringstream istr(line);
	std::string chunk; // コンマ区切りの内容取得用

	// コンマ区切りでの取得
	const int TYPE_ID_INDEX = 4;
	int count = 0;
	while (std::getline(istr, chunk, delim)) // 区切りごとに取得
	{
		++count;
		if (count == TYPE_ID_INDEX) break;
	}

	EnemyType ret = (EnemyType)std::stoi(chunk);
	return ret;
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

