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
}

MenuBase::Select MenuStatus::Update(const PlayerManager* plm)
{
    // �O�̉�ʂɖ߂�
    if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return BACK;


    mCharacterSelect.Update();

    const int playerIndex = mCharacterSelect.GetIndex();
    const Status* status = plm->GetPlayers()[playerIndex]->GetStatus();
    for (int i = 0; i < STATUS_NUM; ++i)
    {
        float width = 0.0f;
        Vector2 pos = {};
        Vector2 center = {};

        // ���Ȃ育�艟���A�ق��ɂ������@����Ες���
        if (i == NAME)
        {
            //mPlNameFont.RenderSet(playerIndex, );
        }
        else if (i == HP || i == MP)
        {
            //pos = Vector2(PLATE_X + FIRST_OFFSET_X, PLATE_Y + FIRST_OFFSET_Y + ADD_OFFSET_Y * i);

            //// �X�e�[�^�X��
            //width = mStatusNameFont.GetWidth(i - 1);
            //mStatusNameFont.RenderSet(i - 1, pos); // k - 1 : ���O�̕� 

            //// ���݂̒l (cur)
            //width = mFontValue.GetWidth(mHealthPlates[i].curHP);
            //pos.x += HealthPlate::CUR_OFFSET_X;
            //center.x = width;
            //mFontValue.RenderSet(mHealthPlates[i].curHP, pos, center);

            //// ��؂� (/)
            //pos.x += HealthPlate::DELIM_OFFSET_X;
            //mDelimFont.RenderSet(0, pos);

            //// �ő�̒l (max)
            //width = mFontValue.GetWidth(mHealthPlates[i].maxHP);
            //pos.x += HealthPlate::DELIM_OFFSET_X + HealthPlate::MAX_OFFSET_X;
            //center.x = width;
            //mFontValue.RenderSet(mHealthPlates[i].maxHP, pos, center);
        }
    }

    return NONE;
}

void MenuStatus::Render()
{
}

void MenuStatus::Release()
{
}
