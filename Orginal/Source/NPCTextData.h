#pragma once
#include <string>
#include <vector>

class NPCTextData
{
	// ���ԍ쐬�֎~
	NPCTextData() = default;
	~NPCTextData() = default;

public:
	struct TextData
	{
		std::wstring name;
		int times;
		bool flag;
		std::vector<std::wstring> texts;
	};

public:
	static void GetTextData(const int charaID, std::vector<TextData>* outData, bool flag);
};