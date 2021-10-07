#include "TransformData.h"

#include "CSVLoader.h"

TransformData::Transform TransformData::GetPLTransform(const int charaID)
{
	const char* filename = "Data/Database/PLTransform.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	Transform ret = {};
	for (auto& line : allLine)
	{
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		int id = std::stoi(chunks[1]);
		if (id == charaID)
		{
			int index = 2; // –¼‘O‚ÆcharaID‚ð–³Ž‹
			ret.diameter = std::stof(chunks[index++]);
			ret.pos.x   = std::stof(chunks[index++]);
			ret.pos.y   = std::stof(chunks[index++]);
			ret.pos.z   = std::stof(chunks[index++]);
			ret.scale.x = std::stof(chunks[index++]);
			ret.scale.y = std::stof(chunks[index++]);
			ret.scale.z = std::stof(chunks[index++]);
			ret.angle.x = std::stof(chunks[index++]);
			ret.angle.y = std::stof(chunks[index++]);
			ret.angle.z = std::stof(chunks[index++]);
			break;
		}
	}

	return ret;
}

TransformData::Transform TransformData::GetEnmTransform(const int charaID)
{
	const char* filename = "Data/Database/EnmTransform.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	Transform ret = {};
	for (auto& line : allLine)
	{
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);
		if (chunks.empty()) continue;

		int id = std::stoi(chunks[1]);
		if (id == charaID)
		{
			int index = 2;// –¼‘O‚ÆcharaID‚ð–³Ž‹
			ret.diameter = std::stof(chunks[index++]);
			ret.pos.x = std::stof(chunks[index++]);
			ret.pos.y = std::stof(chunks[index++]);
			ret.pos.z = std::stof(chunks[index++]);
			ret.scale.x = std::stof(chunks[index++]);
			ret.scale.y = std::stof(chunks[index++]);
			ret.scale.z = std::stof(chunks[index++]);
			ret.angle.x = std::stof(chunks[index++]);
			ret.angle.y = std::stof(chunks[index++]);
			ret.angle.z = std::stof(chunks[index++]);
			break;
		}
	}

	return ret;
}

TransformData::Transform TransformData::GetNPCTransform(const int charaID)
{
	const char* filename = "Data/Database/NPCTransform.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	Transform ret = {};
	for (auto& line : allLine)
	{
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);
		if (chunks.empty()) continue;

		int id = std::stoi(chunks[1]);
		if (id == charaID)
		{
			int index = 2;// –¼‘O‚ÆcharaID‚ð–³Ž‹
			ret.diameter = std::stof(chunks[index++]);
			ret.pos.x = std::stof(chunks[index++]);
			ret.pos.y = std::stof(chunks[index++]);
			ret.pos.z = std::stof(chunks[index++]);
			ret.scale.x = std::stof(chunks[index++]);
			ret.scale.y = std::stof(chunks[index++]);
			ret.scale.z = std::stof(chunks[index++]);
			ret.angle.x = std::stof(chunks[index++]);
			ret.angle.y = std::stof(chunks[index++]);
			ret.angle.z = std::stof(chunks[index++]);
			break;
		}
	}

	return ret;
}
