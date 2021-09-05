#pragma once

#include <vector>
#include <string>
#include <memory>

#include "lib/Font.h"
#include "lib/Vector.h"

class NPC;
class Sprite;

class NPCTalk
{
	static constexpr float TEXT_POS_X = 40.0f;
	static constexpr float TEXT_POS_Y = 44.0f;
	static constexpr float TEXT_ADD_Y = 33.0f;
	static constexpr float TEXT_MAX_WIDTH = 670.0f;
	static const int LINE_MAX = 3;

	std::unique_ptr<Sprite> mTextBoard;
	std::vector<std::wstring> mTexts;
	NPC* mTalkNPC;
	Font mFont;
	Vector3 mPlayerPos;
	int mTextIndex;
	bool mOldTalkingFlag = false;

	void UnSet();
public:
	void Initialize();
	void Update();
	void Render();

	void Set(NPC* npc, const Vector3& playerPos);

	bool IsTalking() const { return mTalkNPC; }
};