#include "SkillSelect.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "DataBase.h"
#include "Define.h"
#include "Singleton.h"

void SkillSelect::Initialize(const int charaID, const Vector2& leftTop)
{
	mSkillBoard = std::make_unique<Sprite>(L"Data/Image/Menu/skill_board.png");
	mSkillSelect = std::make_unique<Sprite>(L"Data/Image/Menu/skill_select.png");

	// �ۑ�����
	mParams = Singleton<DataBase>().GetInstance().GetSkillData()->GetAllSkill(charaID);
	mSkillIndex = 0;
	mBoardPos = leftTop;

	mFont.Initialize();
}

void SkillSelect::Update()
{
	int num = mParams.size();
	if (num > 0)
	{
		int oldIndex = mSkillIndex;
		if (Input::GetButtonTrigger(0, Input::BUTTON::UP)) mSkillIndex = (mSkillIndex + (num - 1)) % num;
		if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSkillIndex = (mSkillIndex + 1) % num;
		if (oldIndex != mSkillIndex) AUDIO.SoundPlay((int)Sound::CURSOR_MOVE);


		// �X�L����
		int loopNum = Math::Min(VERTICAL_SKILL_MAX, num);
		for (int i = 0; i < loopNum; ++i)
		{
			Vector2 pos = {};
			pos.x = mBoardPos.x + SKILLNAME_POS_X;
			pos.y = mBoardPos.y + SKILLNAME_POS_Y + i * FONT_SIZE;

			Vector2 center = Vector2::ZERO;

			mFont.RenderSet(mParams[i].skillName.c_str(), pos, center, Define::FONT_COLOR);
		}

		// �X�L�����(�����ɂ��Ă݂�
		Vector2 pos(mBoardPos.x + SKILLINFO_POS_X, mBoardPos.y + SKILLINFO_POS_Y);
		Vector2 center(Vector2::ZERO);
		mFont.RenderSet(mParams[mSkillIndex].info.c_str(), pos, center);
	}
	else
	{
		// �X�L�����Ȃ�
		Vector2 pos(mBoardPos.x + SKILLINFO_POS_X, mBoardPos.y + SKILLINFO_POS_Y);
		Vector2 center(Vector2::ZERO);
		mFont.RenderSet(L"�X�L���͂���܂���", pos, center);
	}
}

void SkillSelect::Render()
{
	// �v���[�g��`��
	mSkillBoard->Render(mBoardPos, Vector2::ONE, Vector2::ZERO, mSkillBoard->GetSize());

	// �I���摜��`��
	if (mParams.size() > 0)
	{
		Vector2 pos(mBoardPos.x + SKILLNAME_POS_X, mBoardPos.y + SKILLNAME_POS_Y + mSkillIndex * FONT_SIZE);
		mSkillSelect->Render(pos, Vector2::ONE, Vector2::ZERO, mSkillSelect->GetSize());
	}

	// �t�H���g��`��
	mFont.Render();
}
