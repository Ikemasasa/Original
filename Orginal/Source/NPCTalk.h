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
	static const Vector3 NAME_RGB;
	static const Vector3 TEXT_RGB;
	static constexpr float NAME_OFFSET_Y = 5.0f;
	static constexpr float TEXT_POS_X = 271.0f;
	static constexpr float TEXT_POS_Y = 42.0f;
	static constexpr float TEXT_ADD_Y = 32.0f;
	static constexpr float TEXT_MAX_WIDTH = 738.0f;
	static const int LINE_MAX = 4;
	static const int DISP_FRAME = 2; // 2ÉtÉåÅ[ÉÄÇ…1ï∂éöÇ∏Ç¬ï`âÊ

	std::unique_ptr<Sprite> mTextBoard;
	std::vector<std::wstring> mTexts;
	NPC* mTalkNPC;
	Font mFont;
	Vector3 mPlayerPos;
	int mTextIndex;
	int mTextDispNum;
	int mTimer;
	bool mOldTalkingFlag = false;


	void UnSet();
public:
	void Initialize();
	void Update();
	void Render();

	void Set(NPC* npc, const Vector3& playerPos);

	bool IsTalking() const { return mTalkNPC; }
};