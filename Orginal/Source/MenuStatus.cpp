#include "MenuStatus.h"

#include "lib/Input.h"

#include "PlayerManager.h"

void MenuStatus::Initialize(const PlayerManager* plm)
{
    mStatusPlate = std::make_unique<Texture>(L"Data/Image/Menu/status_plate.png");

    mCharacterSelect.Initialize(plm);

    mPlNameFont.Initialize();
    mStatusNameFont.Initialize();
    mFontValue.Initialize();

    // �v���C���[�̖��O�ǉ�
    const auto& players = plm->GetPlayers();
    for (auto& pl : players)
    {
       mPlNameFont.Add(pl->GetStatus()->name.c_str());
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

MenuBase::Select MenuStatus::Update(const PlayerManager* plm)
{
    // �O�̉�ʂɖ߂�
    if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return BACK;


    mCharacterSelect.Update();

    const int playerIndex = mCharacterSelect.GetIndex();
    const Status* status = plm->GetPlayers()[playerIndex]->GetStatus();
    std::vector<int> curValue; // ���݂̃X�e�[�^�X
    std::vector<int> maxValue; // �ő�̃X�e�[�^�X
    curValue.emplace_back(status->hp);
    curValue.emplace_back(status->mp);
    curValue.emplace_back(status->GetTotalAtk());
    curValue.emplace_back(status->GetTotalDef());
    curValue.emplace_back(status->GetTotalSpd());
    maxValue.emplace_back(status->maxHP);
    maxValue.emplace_back(status->maxMP);

    for (int i = 0; i < STATUS_NUM; ++i)
    {
        float width = 0.0f;
        Vector2 pos = {};
        Vector2 center = {};

        // ���Ȃ育�艟���A�ق��ɂ������@����Ες���
        if (i == NAME)
        {
            width = mPlNameFont.GetWidth(status->name.c_str());
            pos = Vector2(PLATE_X + mStatusPlate->GetSize().x * 0.5f, PLATE_Y + FIRST_OFFSET_Y);
            center = Vector2(width * 0.5f, 0.0f);
            mPlNameFont.RenderSet(playerIndex, pos, center);
        }
        else if (i == HP || i == MP)
        {
            pos = Vector2(PLATE_X + FIRST_OFFSET_X, PLATE_Y + FIRST_OFFSET_Y + ADD_OFFSET_Y * i);

            const int statusIndex = i - 1;// i - 1 : ���O�̕� 

            // �X�e�[�^�X��
            width = mStatusNameFont.GetWidth(statusIndex);
            mStatusNameFont.RenderSet(statusIndex, pos); 

            // ���݂̒l (cur)
            width = mFontValue.GetWidth(curValue[statusIndex]);
            pos.x += CUR_OFFSET_X;
            center.x = width;
            mFontValue.RenderSet(curValue[statusIndex], pos, center);

            // ��؂� (/)
            pos.x += DELIM_OFFSET_X;
            mDelimFont.RenderSet(0, pos);

            // �ő�̒l (max)
            width = mFontValue.GetWidth(maxValue[statusIndex]);
            pos.x += DELIM_OFFSET_X + MAX_OFFSET_X;
            center.x = width;
            mFontValue.RenderSet(maxValue[statusIndex], pos, center);
        }
        else
        {
            pos = Vector2(PLATE_X + FIRST_OFFSET_X, PLATE_Y + FIRST_OFFSET_Y + ADD_OFFSET_Y * i);
            const int statusIndex = i - 1;// i - 1 : ���O�̕� 

            // �X�e�[�^�X��
            width = mStatusNameFont.GetWidth(statusIndex);
            mStatusNameFont.RenderSet(statusIndex, pos);

            // ���݂̒l (cur)
            width = mFontValue.GetWidth(curValue[statusIndex]);
            pos.x += CUR_OFFSET_X;
            center.x = width;
            mFontValue.RenderSet(curValue[statusIndex], pos, center);
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
