#include "NPCTextData.h"

#include "lib/ConvertString.h"

#include "CSVLoader.h"

void NPCTextData::GetTextData(const int charaID, std::vector<TextData>* outData, bool flag)
{
	const char* filename = "Data/DataBase/NPCText.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	int readLineNum = 0;
	for (size_t i = 0; i < allLine.size(); ++i)
	{
		auto& line = allLine[i];

		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		const int CHARA_ID_INDEX = 1;
		int id = std::stoi(chunks[CHARA_ID_INDEX]);
		if (id == charaID)
		{
			int index = 0;
			TextData data;
			data.name = ConvertString::ConvertToWstirng(chunks[index++]);
			index++; // キャラIDを飛ばす
			data.times = std::stoi(chunks[index++]);
			readLineNum = std::stoi(chunks[index++]);
			data.flag = (bool)std::stoi(chunks[index++]);

			if (data.flag == flag)
			{
				for (int k = 1; k <= readLineNum; ++k)
				{
					// テキストデータ読み込み
					data.texts.emplace_back(ConvertString::ConvertToWstirng(allLine[i + k]));
				}
				outData->emplace_back(data);
			}

			i += readLineNum;
			readLineNum = 0;
		}
		else if (id > charaID) break;
	}
}