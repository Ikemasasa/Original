#pragma once
#include "Character.h"

#include "NPCTextData.h"

class NPC : public Character
{
	static const int TALK_COUNT_INIT = 1;

	std::vector<NPCTextData::TextData> mTextData;

	int mTalkCount;
	bool mOldFlag;

public:
	NPC(int charaID);
	~NPC();

	void Initialize();
	void Update();
	void Release();

	std::vector<std::wstring> GetTexts();
	std::wstring GetName();
};