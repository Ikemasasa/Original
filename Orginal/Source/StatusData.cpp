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
	// ����agi��ς�������
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
	// �U���o�t
	buffStr.turn = Math::Max(0, buffStr.turn - 1);
	if (buffStr.turn <= 0) buffStr.rate = 1.0f;

	// �h��o�t
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

			// �G�^�C�v�A�^�C�vID�𖳎�
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

	std::string line;  // 1�s�擾�p

	// �^�O�𖳎�
	std::getline(fin, line);

	for (int i = DataBase::ENM_ID_START; i <= id; ++i)
	{
		std::getline(fin, line);
	}

	const char delim = ','; // ��؂蕶��
	std::istringstream istr(line);
	std::string chunk; // �R���}��؂�̓��e�擾�p

	// �R���}��؂�ł̎擾
	const int TYPE_ID_INDEX = 4;
	int count = 0;
	while (std::getline(istr, chunk, delim)) // ��؂育�ƂɎ擾
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

