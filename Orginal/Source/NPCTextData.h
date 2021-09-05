#pragma once
#include <string>
#include <vector>

class NPCTextData
{
	// é¿ë‘çÏê¨ã÷é~
	NPCTextData() = default;
	~NPCTextData() = default;

public:
	struct TextData
	{
		int times;
		bool flag;
		std::vector<std::wstring> texts;
	};

public:
	static void GetTextData(const int charaID, std::vector<TextData>* outData, bool flag);
};