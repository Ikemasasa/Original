#include "NPCTalk.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "NPC.h"

void NPCTalk::Initialize()
{
	mTextBoard = std::make_unique<Sprite>(L"Data/Image/text_board.png");
	mFont.Initialize();
}

void NPCTalk::Update()
{
	// データがないならreturn
	if (!mTalkNPC) return;

	// 1フレームだけスルーする
	if (!mOldTalkingFlag)
	{
		mOldTalkingFlag = true;
		return;
	}

	// 向き更新
	Vector3 d = mPlayerPos - mTalkNPC->GetPos();
	mTalkNPC->CorrectionAngle(d.GetNormalize());

	// 会話進めるかチェック
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		Audio::SoundPlay((int)Sound::SELECT);
		++mTextIndex;
		if (mTextIndex >= mTexts.size())
		{
			UnSet();
		}
	}
}

void NPCTalk::Render()
{
	if (!mTalkNPC) return;

	// メッセージウィンドウを描画
	const float BOARD_POS_ADJUST_Y = 100.0f;
	Vector2 size(mTextBoard->GetSize());
	Vector2 pos((Define::SCREEN_WIDTH - size.x) / 2.0f, Define::SCREEN_HEIGHT - size.y - BOARD_POS_ADJUST_Y);
	mTextBoard->Render(pos, Vector2::ONE, Vector2::ZERO, size);

	// テキストを数行に分ける
	const wchar_t* text = mTexts[mTextIndex].c_str();
	size_t textSize = mTexts[mTextIndex].size();
	std::wstring str[LINE_MAX];
	int strIndex = 0;
	for (size_t i = 0; i < textSize; ++i)
	{
		str[strIndex] += text[i];
		if (mFont.GetWidth(str[strIndex].c_str()) >= TEXT_MAX_WIDTH)
		{
			strIndex = Math::Min(strIndex + 1, LINE_MAX - 1);
		}
	}

	for (int i = 0; i < LINE_MAX; ++i)
	{
		Vector2 textPos(pos + Vector2(TEXT_POS_X, TEXT_POS_Y));
		textPos.y += TEXT_ADD_Y * i;
		mFont.RenderSet(str[i].c_str(), textPos, Vector2::ZERO, Define::FONT_COLOR);
	}

	mFont.Render();
}

void NPCTalk::Set(NPC* npc, const Vector3& playerPos)
{
	if (mTalkNPC) return;

	mTalkNPC = npc;
	mTexts = npc->GetTexts();
	mTextIndex = 0;
	mPlayerPos = playerPos;
}

void NPCTalk::UnSet()
{
	mTalkNPC = nullptr;
	mTexts.clear();
	mFont.Release();
	mTextIndex = 0;
	mOldTalkingFlag = false;
}