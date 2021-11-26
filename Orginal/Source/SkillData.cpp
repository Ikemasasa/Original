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

	// 全行取得
	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	for (size_t i = 0; i < allLine.size(); ++i)
	{
		// コンマ区切りに分割
		std::vector<std::string> chunks;
		loader.GetChunks(allLine[i], &chunks);

		// 追加
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
	// charaIDからどの種類(敵かプレイヤーか等)を判別
	const char* filename = nullptr;
	int pl = abs(charaID - DataBase::PL_ID_START);
	int enm = abs(charaID - DataBase::ENM_ID_START);
	int check = Math::Min(pl, enm);
	if (check == pl)  filename = "Data/DataBase/PLSkill.csv";
	if (check == enm) filename = nullptr;

	// ファイルオープン
	CSVLoader loader;
	loader.Open(filename);

	// スキルを取得する
	const int CHARA_ID_INDEX = 1;
	const int SKILL_ID_INDEX = 3;
	std::vector<const SkillData::BaseData*> ret;
	std::string line;
	while (true)
	{
		// 1行取得
		loader.GetNextLine(&line);

		// EOFならbreak;
		if (loader.IsEndFile()) break;

		// コンマ区切りで分割
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);
		
		// charaIDが一致したら
		if (std::stoi(chunks[CHARA_ID_INDEX]) == charaID)
		{
			// スキル情報を取得
			int skillID = std::stoi(chunks[SKILL_ID_INDEX]);
			ret.push_back(SkillData::GetBaseData((SkillData::Kind)skillID));
		}
		else
		{
			// charaIDが一致せず、retがエンプティじゃないならbreak;
			if (!ret.empty()) break;
		}


	}

	loader.Close();

	return ret;
}

BuffSkillData::Param BuffSkillData::GetParam(const size_t id)
{
	const char* filename = "Data/DataBase/BuffSkill.csv";

	// ファイルを開く
	CSVLoader loader;
	loader.Open(filename);

	// 全行取得
	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	// idと一致する行を探す
	Param ret = {};
	const int ID_INDEX = 1;
	for (const auto& line : allLine)
	{
		// lineのコンマ区切りを取得
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		// IDが一致したら
		if (std::stoi(chunks[ID_INDEX]) == id)
		{
			// 戻り値を設定

			// ベースデータ取得
			ret.base = SkillData::GetBaseData((SkillData::Kind)id);

			// csvより取得
			int index = ID_INDEX + 1; // 名前、IDを飛ばす
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

	// ファイルを開く
	CSVLoader loader;
	loader.Open(filename);

	// 全行取得
	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	// idと一致する行を探す
	Param ret = {};
	const int ID_INDEX = 1;
	for (const auto& line : allLine)
	{
		// lineのコンマ区切りを取得
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		// IDが一致したら
		if (std::stoi(chunks[ID_INDEX]) == id)
		{
			// 戻り値を設定

			// ベースデータ取得
			ret.base = SkillData::GetBaseData((SkillData::Kind)id);

			// csvより取得
			int index = ID_INDEX + 1; // 名前、IDを飛ばす
			ret.skillRate = std::stof(chunks[index++]);
			ret.hitNum = std::stoi(chunks[index++]);

			break;
		}
	}

	loader.Close();

	return ret;
}
