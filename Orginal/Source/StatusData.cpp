#include "StatusData.h"

#include "lib/ConvertString.h"

#include "CSVLoader.h"
#include "DataBase.h"
#include "DamageCalculator.h"

// static メンバ
std::vector<Status> StatusData::mPLStatus;


//---------------------------------------------
// Status
//---------------------------------------------
int Status::GetAtk() const
{
	int atk = 0;
	const EquipmentData::Param* param = equipments.GetParam(EquipmentData::WEAPON);
	if (param) atk = param->atk;
	atk = (str + atk) * buffAtk.rate; // バフ計算
	atk -= (str + atk) * (1.0f - debuffAtk.rate); // デバフ計算

	return atk;
}

int Status::GetDef() const
{
	int def = 0;
	const EquipmentData::Param* param = equipments.GetParam(EquipmentData::ARMOR);
	if (param) def = param->def;
	def = (vit + def) * buffDef.rate;// バフ計算
	def -= (vit + def) * (1.0f - debuffDef.rate); // デバフ計算

	if (guardFlag) def *= DamageCalculator::GUARD_BUFF_RATE;

	return def;
}

int Status::GetSpd() const
{
	// 今はagiを変えすだけ
	return agi;
}

void Status::SetBuffAtkRate(const float rate, const int turn)
{
	buffAtk.rate = Math::Max(buffAtk.rate, rate);
	buffAtk.turn = Math::Max(buffAtk.turn, turn);
}

void Status::SetBuffDefRate(const float rate, const int turn)
{
	buffDef.rate = Math::Max(buffDef.rate, rate);
	buffDef.turn = Math::Max(buffDef.turn, turn);
}

void Status::SetDebuffAtkRate(const float rate, const int turn)
{
	debuffAtk.rate = Math::Min(debuffAtk.rate, rate);
	debuffAtk.turn = Math::Max(debuffAtk.turn, turn);
}

void Status::SetDebuffDefRate(const float rate, const int turn)
{
	debuffDef.rate = Math::Min(debuffDef.rate, rate);
	debuffDef.turn = Math::Max(debuffDef.turn, turn);
}

void Status::AdvanceBuffTurn()
{
	// 攻撃バフ
	buffAtk.turn = Math::Max(0, buffAtk.turn - 1);
	if (buffAtk.turn <= 0) buffAtk.rate = 1.0f;

	// 防御バフ
	buffDef.turn = Math::Max(0, buffDef.turn - 1);
	if (buffDef.turn <= 0) buffDef.rate = 1.0f;

	// 攻撃デバフ
	debuffAtk.turn = Math::Max(0, debuffAtk.turn - 1);
	if (debuffAtk.turn <= 0) debuffAtk.rate = 1.0f;

	// 防御デバフ
	debuffDef.turn = Math::Max(0, debuffDef.turn - 1);
	if (debuffDef.turn <= 0) debuffDef.rate = 1.0f;
}

void Status::ResetBuff()
{
	buffAtk.rate = 1.0f;
	buffAtk.turn = 0;
	buffDef.rate = 1.0f;
	buffDef.turn = 0;

	debuffAtk.rate = 1.0f;
	debuffAtk.turn = 0;
	debuffDef.rate = 1.0f;
	debuffDef.turn = 0;

	guardFlag = false;
}

//---------------------------------------------
// StatusData
//---------------------------------------------
StatusData::StatusData()
{
	mPLStatus.clear();

	LoadPLStatus();
}

StatusData::~StatusData()
{
	mPLStatus.clear();
}

void StatusData::LoadPLStatus()
{
	const char* filename = "Data/DataBase/PLStatus.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	for (const auto& line : allLine)
	{
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		if (chunks.size() > 0)
		{
			int index = 0;
			Status status;
			status.name = ConvertString::ConvertToWstring(chunks[index++]);
			status.id = std::stoi(chunks[index++]);
			status.hp = std::stoi(chunks[index++]);
			status.maxHP = status.hp;
			status.mp = std::stoi(chunks[index++]);
			status.maxMP = status.mp;
			status.str = std::stoi(chunks[index++]);
			status.vit = std::stoi(chunks[index++]);
			status.agi = std::stoi(chunks[index++]);
			status.ResetBuff();

			mPLStatus.emplace_back(status);
		}
	}

	loader.Close();
}

Status StatusData::GetEnmStatus(size_t id)
{
	const char* filename = "Data/DataBase/EnmStatus.csv";
	
	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	// 目的のラインを取得する
	const int LINE_INDEX = id - DataBase::ENM_ID_START;

	std::vector<std::string> chunks;
	loader.GetChunks(allLine[LINE_INDEX], &chunks);
	Status ret = {};
	if (chunks.size() > 0)
	{
		int index = 0;
		ret.name = ConvertString::ConvertToWstring(chunks[index++]);
		ret.id = std::stoi(chunks[index++]);
		index += 1; // エネミータイプを無視する
		ret.hp = std::stoi(chunks[index++]);
		ret.maxHP = ret.hp;
		ret.mp = std::stoi(chunks[index++]);
		ret.maxMP = ret.mp;
		ret.str = std::stoi(chunks[index++]);
		ret.vit = std::stoi(chunks[index++]);
		ret.agi = std::stoi(chunks[index++]);
		ret.ResetBuff();
	}


	loader.Close();

	return ret;
}

StatusData::EnemyType StatusData::GetEnmType(size_t id) 
{
	const char* filename = "Data/DataBase/EnmStatus.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	// 目的のラインを取得する
	const int LINE_INDEX = id - DataBase::ENM_ID_START;

	std::vector<std::string> chunks;
	loader.GetChunks(allLine[LINE_INDEX], &chunks);

	// タイプを取得
	const int TYPE_INDEX = 2;
	EnemyType ret = (EnemyType)std::stoi(chunks[TYPE_INDEX]);

	return ret;
}

void StatusData::Initialize()
{
	LoadPLStatus();
}

void StatusData::Release()
{
	mPLStatus.clear();
}

Status StatusData::GetPLStatus(size_t id) 
{
	return mPLStatus[id - DataBase::PL_ID_START];
}

void StatusData::SetPLStatus(size_t charaID, const Status& status)
{
	mPLStatus[charaID - DataBase::PL_ID_START] = status;
}

