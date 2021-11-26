#include "Music.h"

#include "lib/Audio.h"
#include "lib/ConvertString.h"

#include "CSVLoader.h"

void Music::Initialize()
{
	const char* filename = "Data/DataBase/MusicData.csv";

	CSVLoader loader;
	loader.Open(filename);

	// 全行取得
	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);
	
	// 読み込み
	for (size_t i = 0; i < allLine.size(); ++i)
	{
		// コンマで分割
		std::vector<std::string> chunks;
		loader.GetChunks(allLine[i], &chunks);

		// ロード
		if (chunks.size() > 0)
		{
			std::wstring musicFilename = L"Data/Audio/Music/" + ConvertString::ConvertToWstring(chunks[0]);
			Audio::LoadMusic(i, musicFilename.c_str());
		}
	}

	loader.Close();
}

void Music::Play(Music::Kind kind)
{
	Audio::MusicPlay((int)kind);
}

void Music::StopAll()
{
	Audio::MusicStopAll();
}

void Music::Pause()
{
	Audio::MusicPause();
}

void Music::Resume()
{
	Audio::MusicResume();
}