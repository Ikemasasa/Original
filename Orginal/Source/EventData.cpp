#include "EventData.h"

#include "CSVLoader.h"

std::string EventData::GetEventFilePath(const int eventID)
{
	const char* filename = "Data/DataBase/EventData.csv";

	CSVLoader loader;
	loader.Open(filename);

	for (int i = 0; i < EventID::EVENT_NUM; ++i)
	{
		std::string line;
		loader.GetNextLine(&line);

		std::vector<std::string> chunks;
		loader.GetChunks(line, &chunks);

		const int ID_INDEX = 1;
		const int FILEPATH_INDEX = 2;
		if (chunks.size() > 0)
		{
			int id = std::stoi(chunks[ID_INDEX]);
			if (id == eventID)
			{
				return "Data/DataBase/Event/" + chunks[FILEPATH_INDEX] + ".xml";
			}
		}
	}

	loader.Close();
}
