#include "SkillData.h"

#include "lib/ConvertString.h"
#include "lib/Math.h"

#include "CSVLoader.h"
#include "DataBase.h"

std::vector<SkillData::BaseData> SkillData::mSkills;

void SkillData::Initialize()
{
	mSkills.clear();
	LoadSkillData();
}

void SkillData::Release()
{
	mSkills.clear();
}

void SkillData::LoadSkillData()
{
	const char* filename = "Data/DataBase/SkillBase.csv";

	CSVLoader loader;
	loader.Open(filename);

	// �S�s�擾
	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	for (size_t i = 0; i < allLine.size(); ++i)
	{
		// �R���}��؂�ɕ���
		std::vector<std::string> chunks;
		loader.GetChunks(allLine[i], &chunks);

		// �ǉ�
		if (chunks.size() > 0)
		{
			int index = 0;
			BaseData base;
			base.name = ConvertString::ConvertToWstring(chunks[index++]);
			base.id		   = std::stoi(chunks[index++]);
			base.useMP     = std::stoi(chunks[index++]);
			base.type      = (Type)std::stoi(chunks[index++]);
			base.target	   = (Target)std::stoi(chunks[index++]);
			base.range = (Range)std::stoi(chunks[index++]);
			base.rate	   = (Rate)std::stoi(chunks[index++]);
			base.effectID  = std::stoi(chunks[index++]);
			base.soundID   = std::stoi(chunks[index++]);
			base.info = ConvertString::ConvertToWstring(chunks[index++]);

			mSkills.emplace_back(base);
		}
	}

	loader.Close();
}

std::vector<const SkillData::BaseData*> SkillData::GetCharaSkill(int charaID)
{
	// charaID����ǂ̎��(�G���v���C���[����)�𔻕�
	const char* filename = nullptr;
	int pl = abs(charaID - DataBase::PL_ID_START);
	int enm = abs(charaID - DataBase::ENM_ID_START);
	int check = Math::Min(pl, enm);
	if (check == pl)  filename = "Data/DataBase/PLSkill.csv";
	if (check == enm) filename = nullptr;

	// �t�@�C���I�[�v��
	CSVLoader loader;
	loader.Open(filename);

	// �X�L�����擾����
	const int CHARA_ID_INDEX = 1;
	const int SKILL_ID_INDEX = 3;
	std::vector<const SkillData::BaseData*> ret;
	std::string line;
	while (true)
	{
		// 1�s�擾
		loader.GetNextLine(&line);

		// EOF�Ȃ�break;
		if (loader.IsEndFile()) break;

		// �R���}��؂�ŕ���
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);
		
		// charaID����v������
		if (std::stoi(chunks[CHARA_ID_INDEX]) == charaID)
		{
			// �X�L�������擾
			int skillID = std::stoi(chunks[SKILL_ID_INDEX]);
			ret.push_back(SkillData::GetBaseData((SkillData::Kind)skillID));
		}
		else
		{
			// charaID����v�����Aret���G���v�e�B����Ȃ��Ȃ�break;
			if (!ret.empty()) break;
		}


	}

	loader.Close();

	return ret;
}

BuffSkillData::Param BuffSkillData::GetParam(const size_t id)
{
	const char* filename = "Data/DataBase/BuffSkill.csv";

	// �t�@�C�����J��
	CSVLoader loader;
	loader.Open(filename);

	// �S�s�擾
	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	// id�ƈ�v����s��T��
	Param ret = {};
	const int ID_INDEX = 1;
	for (const auto& line : allLine)
	{
		// line�̃R���}��؂���擾
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		// ID����v������
		if (std::stoi(chunks[ID_INDEX]) == id)
		{
			// �߂�l��ݒ�

			// �x�[�X�f�[�^�擾
			ret.base = SkillData::GetBaseData((SkillData::Kind)id);

			// csv���擾
			int index = ID_INDEX + 1; // ���O�AID���΂�
			ret.turn = std::stoi(chunks[index++]);
			ret.atkValue = std::stof(chunks[index++]);
			ret.defValue = std::stof(chunks[index++]);

			break;
		}
	}

	loader.Close();

	return ret;
}

AttackSkillData::Param AttackSkillData::GetParam(const size_t id)
{
	const char* filename = "Data/DataBase/AttackSkill.csv";

	// �t�@�C�����J��
	CSVLoader loader;
	loader.Open(filename);

	// �S�s�擾
	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	// id�ƈ�v����s��T��
	Param ret = {};
	const int ID_INDEX = 1;
	for (const auto& line : allLine)
	{
		// line�̃R���}��؂���擾
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		// ID����v������
		if (std::stoi(chunks[ID_INDEX]) == id)
		{
			// �߂�l��ݒ�

			// �x�[�X�f�[�^�擾
			ret.base = SkillData::GetBaseData((SkillData::Kind)id);

			// csv���擾
			int index = ID_INDEX + 1; // ���O�AID���΂�
			ret.skillRate = std::stof(chunks[index++]);
			ret.hitNum = std::stoi(chunks[index++]);

			break;
		}
	}

	loader.Close();

	return ret;
}
