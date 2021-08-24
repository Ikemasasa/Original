#include "SkillData.h"

#include <fstream>
#include <sstream>

#include "lib/ConvertString.h"

std::vector<SkillData::SkillParam> SkillData::GetAllSkill(const int charaID)
{
	const char* filename = "Data/DataBase/PLSkill.csv";

	std::vector<SkillData::SkillParam> ret;

	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) return ret;

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
			if (std::stoi(data[3]) == charaID)
			{
				int index = 0;
				SkillData::SkillParam param;
				param.skillName = ConvertString::ConvertToWstirng(data[index++]);
				param.skillID = std::stoi(data[index++]);
				index += 2;
				param.useMP = std::stoi(data[index++]);
				param.turn = std::stoi(data[index++]);
				param.type = (Type)std::stoi(data[index++]);
				param.target = (Target)std::stoi(data[index++]);
				param.targetNum = (TargetNum)std::stoi(data[index++]);
				param.rate = (Rate)std::stoi(data[index++]);
				param.atkValue = std::stof(data[index++]);
				param.defValue = std::stof(data[index++]);
				param.info = ConvertString::ConvertToWstirng(data[index++]);

				ret.push_back(param);
			}
		}
	}

	return ret;
}
