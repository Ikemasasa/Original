#include "MotionCollision.h"

#include "CSVLoader.h"

MotionCollision::ColData MotionCollision::GetPLMotionCollision(const int charaID)
{
	const char* filename = "Data/DataBase/PLMotionCollision.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	ColData colData = {};
	for (const auto& line : allLine)
	{
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		const int ID_INDEX = 1;
		int id = std::stoi(chunks[ID_INDEX]);
		if (id == charaID)
		{
			int index = 3; // キャラ名、キャラID、モーションIDを飛ばす

			colData.boneName = chunks[index++];
			colData.beginFrame = std::stoi(chunks[index++]);
			colData.endFrame = std::stoi(chunks[index++]);
			colData.radius = std::stof(chunks[index++]);
			break;
		}
	}

	return colData;
}

MotionCollision::ColData MotionCollision::GetEnmMotionCollision(const int charaID)
{
	const char* filename = "Data/DataBase/EnmMotionCollision.csv";

	CSVLoader loader;
	loader.Open(filename);

	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	ColData colData = {};
	for (const auto& line : allLine)
	{
		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		const int ID_INDEX = 1;
		int id = std::stoi(chunks[ID_INDEX]);
		if (id == charaID)
		{
			int index = 3; // キャラ名、キャラID、モーションIDを飛ばす

			colData.boneName = chunks[index++];
			colData.beginFrame = std::stoi(chunks[index++]);
			colData.endFrame = std::stoi(chunks[index++]);
			colData.radius = std::stof(chunks[index++]);
			break;
		}
	}

	return colData;
}
