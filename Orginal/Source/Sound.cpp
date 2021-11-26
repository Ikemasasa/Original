#include "Sound.h"

#include "lib/Audio.h"
#include "lib/ConvertString.h"

#include "CSVLoader.h"

void Sound::Initialize()
{
	const char* filename = "Data/DataBase/SoundData.csv";

	CSVLoader loader;
	loader.Open(filename);

	// �S�s�擾
	std::vector<std::string> allLine;
	loader.GetAllLine(&allLine);

	// �ǂݍ���
	for (size_t i = 0; i < allLine.size(); ++i)
	{
		// �R���}�ŕ���
		std::vector<std::string> chunks;
		loader.GetChunks(allLine[i], &chunks);

		// ���[�h
		if (chunks.size() > 0)
		{
			std::wstring musicFilename = L"Data/Audio/Sound/" + ConvertString::ConvertToWstring(chunks[1]);
			Audio::LoadSound(i, musicFilename.c_str());
		}
	}

	loader.Close();
}

void Sound::Play(Sound::Kind kind)
{
	Audio::SoundPlay((int)kind);
}

void Sound::Stop(Sound::Kind kind)
{
	Audio::SoundStop((int)kind);
}
