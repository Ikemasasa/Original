#include "NPCTalk.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "NPC.h"

const Vector3 NPCTalk::NAME_RGB = { 1.0f, 0.85f, 0.2f };
const Vector3 NPCTalk::TEXT_RGB = { 0.9f, 0.9f, 0.9f };

void NPCTalk::Initialize()
{
	mTextBoard = std::make_unique<Sprite>(L"Data/Image/text_board.png");
	mFont.Initialize();
}

void NPCTalk::Update()
{
	// �f�[�^���Ȃ��Ȃ�return
	if (!mTalkNPC) return;

	// 1�t���[�������X���[����
	if (!mOldTalkingFlag)
	{
		mOldTalkingFlag = true;
		return;
	}

	// �����X�V
	Vector3 d = mPlayerPos - mTalkNPC->GetPos();
	mTalkNPC->CorrectionAngle(d.GetNormalize());

	// �\�����X�V
	if (mTextDispNum < mTexts[mTextIndex].size())
	{
		mTimer = (mTimer + 1) % DISP_FRAME;
		if (mTimer == 0)
		{
			++mTextDispNum;
		}
	}

	// ��b�i�߂邩�`�F�b�N
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// �������蒆�Ȃ�S������\������
		if (mTextDispNum < mTexts[mTextIndex].size())
		{
			mTextDispNum = mTexts[mTextIndex].size();
		}
		else
		{
			Audio::SoundPlay((int)Sound::SELECT);
			++mTextIndex;
			mTimer = 0;
			mTextDispNum = 1;

			if (mTextIndex >= mTexts.size())
			{
				UnSet();
			}
		}
	}


}

void NPCTalk::Render()
{
	if (!mTalkNPC) return;

	// ���b�Z�[�W�E�B���h�E��`��
	const float BOARD_POS_ADJUST_Y = 25.0f;
	Vector2 size(mTextBoard->GetSize());
	Vector2 pos((Define::SCREEN_WIDTH - size.x) / 2.0f, Define::SCREEN_HEIGHT - size.y - BOARD_POS_ADJUST_Y);
	mTextBoard->Render(pos, Vector2::ONE, Vector2::ZERO, size);

	// ���O
	Vector2 namePos(Define::SCREEN_WIDTH / 2.0f, pos.y + NAME_OFFSET_Y);
	mFont.RenderSet(mTalkNPC->GetName().c_str(), namePos, Vector2(0.5f, 0.0f), Vector4(NAME_RGB, 1.0f));

	// �e�L�X�g�𐔍s�ɕ�����
	const wchar_t* text = mTexts[mTextIndex].c_str();
	size_t textSize = mTexts[mTextIndex].size();
	std::wstring str[LINE_MAX];
	int strIndex = 0;
	for (size_t i = 0; i < mTextDispNum; ++i)
	{
		str[strIndex] += text[i];
		if (mFont.GetWidth(str[strIndex].c_str()) >= TEXT_MAX_WIDTH)
		{
			strIndex = Math::Min(strIndex + 1, LINE_MAX - 1);
		}
	}

	for (int i = 0; i < LINE_MAX; ++i)
	{
		if (str[i].empty()) continue;
		Vector2 textPos(pos + Vector2(TEXT_POS_X, TEXT_POS_Y));
		textPos.y += TEXT_ADD_Y * i;
		mFont.RenderSet(str[i].c_str(), textPos, Vector2::ZERO, Vector4(TEXT_RGB, 1.0f));
	}

	mFont.Render();
}

void NPCTalk::Set(NPC* npc, const Vector3& playerPos)
{
	if (mTalkNPC) return;

	mTalkNPC = npc;
	mTexts = npc->GetTexts();
	mTextIndex = 0;
	mTextDispNum = 1;
	mPlayerPos = playerPos;
}

void NPCTalk::UnSet()
{
	mTalkNPC = nullptr;
	mTexts.clear();
	mFont.Release();
	mTextIndex = 0;
	mTextDispNum = 1;

	mOldTalkingFlag = false;
}