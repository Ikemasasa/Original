#include "MenuStatus.h"

#include "lib/Input.h"

#include "DataBase.h"
#include "Define.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Singleton.h"
#include "StatusData.h"

void MenuStatus::Initialize(const PlayerManager* plm)
{
    mStatusPlate = std::make_unique<Sprite>(L"Data/Image/Menu/status_plate.png");

    mCharacterSelect.Initialize(plm);

    mPlNameFont.Initialize();
    mStatusNameFont.Initialize();
    mFontValue.Initialize();

    // �v���C���[�̖��O�ǉ�
    for (size_t i = 0; i < plm->GetNum(); ++i)
    {
        std::wstring name = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(plm->GetPlayer(i)->GetCharaID()).GetName();
        mPlNameFont.Add(name.c_str());
    }

    // �X�e�[�^�X���ǉ�
    const wchar_t* statusName[] =
    {
        L"HP",
        L"MP",
        L"�U����",
        L"�h���",
        L"�f����"
    };
    int arrayNum = ARRAYSIZE(statusName);
    for (int i = 0; i < arrayNum; ++i)
    {
        mStatusNameFont.Add(statusName[i]);
    }

    mDelimFont.Add(L"/");
}

MenuBase::Select MenuStatus::Update(PlayerManager* plm)
{
    // �O�̉�ʂɖ߂�
    if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return BACK;

    // ���艹��炳�Ȃ�
    AUDIO.SoundStop((int)Sound::SELECT);

    mCharacterSelect.Update();

    const int playerIndex = mCharacterSelect.GetIndex();
    int charaID = plm->GetPlayer(playerIndex)->GetCharaID();
    const Status status = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(charaID);
    std::vector<int> curValue; // ���݂̃X�e�[�^�X
    std::vector<int> maxValue; // �ő�̃X�e�[�^�X
    curValue.emplace_back(status.GetHP());
    curValue.emplace_back(status.GetMP());
    curValue.emplace_back(status.GetAtk());
    curValue.emplace_back(status.GetDef());
    curValue.emplace_back(status.GetSpd());
    maxValue.emplace_back(status.GetMaxHP());
    maxValue.emplace_back(status.GetMaxMP());

    for (int i = 0; i < STATUS_NUM; ++i)
    {
        float width = 0.0f;
        Vector2 pos = {};
        Vector2 center = {};

        // ���Ȃ育�艟���A�ق��ɂ������@����Ες���
        if (i == NAME)
        {
            width = mPlNameFont.GetWidth(status.GetName().c_str());
            pos = Vector2(PLATE_X + mStatusPlate->GetSize().x * 0.5f, PLATE_Y + FIRST_OFFSET_Y);
            center = Vector2(width * 0.5f, 0.0f);
            mPlNameFont.RenderSet(playerIndex, pos, center, Define::FONT_COLOR);
        }
        else if (i == HP || i == MP)
        {
            pos = Vector2(PLATE_X + FIRST_OFFSET_X, PLATE_Y + FIRST_OFFSET_Y + ADD_OFFSET_Y * i);

            const int statusIndex = i - 1;// i - 1 : ���O�̕� 

            // �X�e�[�^�X��
            width = mStatusNameFont.GetWidth(statusIndex);
            mStatusNameFont.RenderSet(statusIndex, pos, Vector2::ZERO, Define::FONT_COLOR);

            // ���݂̒l (cur)
            width = mFontValue.GetWidth(curValue[statusIndex]);
            pos.x += CUR_OFFSET_X;
            center.x = width;
            mFontValue.RenderSet(curValue[statusIndex], pos, center, Define::FONT_COLOR);

            // ��؂� (/)
            pos.x += DELIM_OFFSET_X;
            mDelimFont.RenderSet(0, pos, Vector2::ZERO, Define::FONT_COLOR);

            // �ő�̒l (max)
            width = mFontValue.GetWidth(maxValue[statusIndex]);
            pos.x += DELIM_OFFSET_X + MAX_OFFSET_X;
            center.x = width;
            mFontValue.RenderSet(maxValue[statusIndex], pos, center, Define::FONT_COLOR);
        }
        else
        {
            pos = Vector2(PLATE_X + FIRST_OFFSET_X, PLATE_Y + FIRST_OFFSET_Y + ADD_OFFSET_Y * i);
            const int statusIndex = i - 1;// i - 1 : ���O�̕� 

            // �X�e�[�^�X��
            width = mStatusNameFont.GetWidth(statusIndex);
            mStatusNameFont.RenderSet(statusIndex, pos, Vector2::ZERO, Define::FONT_COLOR);

            // ���݂̒l (cur)
            width = mFontValue.GetWidth(curValue[statusIndex]);
            pos.x += CUR_OFFSET_X;
            center.x = width;
            mFontValue.RenderSet(curValue[statusIndex], pos, center, Define::FONT_COLOR);
        }
    }

    return NONE;
}

void MenuStatus::Render()
{
    // �摜�n�`��
    {
        Vector2 pos(PLATE_X, PLATE_Y);
        Vector2 scale(1.0f, 1.0f);
        Vector2 texPos(0.0f, 0.0f);
        Vector2 size(mStatusPlate->GetSize());
        mStatusPlate->Render(pos, scale, texPos, size);
        mCharacterSelect.Render(pos);
    }

    // �t�H���g�`��
    mPlNameFont.Render();
    mStatusNameFont.Render();
    mDelimFont.Render();
    mFontValue.Render();
}

void MenuStatus::Release()
{
    mPlNameFont.Release();
    mStatusNameFont.Release();
    mDelimFont.Release();
    mFontValue.Release();
}
