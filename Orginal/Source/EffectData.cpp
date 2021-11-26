#include "EffectData.h"

#include <codecvt>

#include "lib/ConvertString.h"

#include "CSVLoader.h"

EffectData::Param EffectData::GetParam(EffectData::Kind kind)
{
	const char* filename = "Data/DataBase/EffectData.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::string line;
	for (int i = 0; i <= (int)kind; ++i)
	{
		loader.GetNextLine(&line);
	}

	std::vector<std::string> chunks;
	loader.GetChunks(line, &chunks);

	Param ret = {};
	if (chunks.size() > 0)
	{
		int index = 0;
		ret.slotID = std::stoi(chunks[index++]);

		std::wstring wstr = ConvertString::ConvertToWstring(chunks[index++]);
		std::u16string u16str(wstr.begin(), wstr.end());
		ret.filename = u"Data/Effect/" + u16str;
		ret.scale = std::stof(chunks[index++]);
		ret.speed = std::stof(chunks[index++]);
	}

	loader.Close();

	return ret;
}
