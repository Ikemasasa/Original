#include "SkillData.h"

#include "lib/ConvertString.h"

#include "CSVLoader.h"

std::vector<SkillData::SkillParam> SkillData::GetAllSkill(const int charaID)
{
	const char* filename = "Data/DataBase/PLSkill.csv";

	std::vector<SkillData::SkillParam> ret;

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	for (const auto& line : allLine)
	{
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		const int CHARA_ID_INDEX = 3;
		int id = std::stoi(chunks[CHARA_ID_INDEX]);
		if (id == charaID)
		{
			int index = 0;
			SkillParam param;
			param.skillName = ConvertString::ConvertToWstring(chunks[index++]);
			param.skillID	= std::stoi(chunks[index++]);
			index += 2; // –¼‘O‚Æ‚«‚á‚çID‚ð–³Ž‹
			param.useMP		= std::stoi(chunks[index++]);
			param.turn		= std::stoi(chunks[index++]);
			param.type		= (Type)std::stoi(chunks[index++]);
			param.target	= (Target)std::stoi(chunks[index++]);
			param.targetNum = (TargetNum)std::stoi(chunks[index++]);
			param.rate		= (Rate)std::stoi(chunks[index++]);
			param.atkValue  = std::stof(chunks[index++]);
			param.defValue  = std::stof(chunks[index++]);
			param.info		= ConvertString::ConvertToWstring(chunks[index++]);

			ret.emplace_back(param);
		}
		if (id > charaID) break; // id‚ªcharaiD‚æ‚è‘å‚«‚­‚È‚Á‚½‚çbreak;
	}

	loader.Close();

	return ret;
}
