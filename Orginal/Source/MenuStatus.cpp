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

    // プレイヤーの名前追加
    const auto& players = plm->GetPlayers();
    for (auto& pl : players)
    {
       mPlNameFont.Add(pl->GetStatus()->name.c_str());
    }

    // ステータス名追加
    const wchar_t* statusName[] =
    {
        L"HP",
        L"MP",
        L"攻撃力",
        L"防御力",
        L"素早さ"
    };
    int arrayNum = ARRAYSIZE(statusName);
    for (int i = 0; i < arrayNum; ++i)
    {
        mStatusNameFont.Add(statusName[i]);
    }
}

MenuBase::Select MenuStatus::Update(const PlayerManager* plm)
{
    // 前の画面に戻す
    if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return BACK;


    mCharacterSelect.Update();

    const int playerIndex = mCharacterSelect.GetIndex();
    const Status* status = plm->GetPlayers()[playerIndex]->GetStatus();
    for (int i = 0; i < STATUS_NUM; ++i)
    {
        float width = 0.0f;
        Vector2 pos = {};
        Vector2 center = {};

        // かなりごり押し、ほかにいい方法あれば変える
        if (i == NAME)
        {
            //mPlNameFont.RenderSet(playerIndex, );
        }
        else if (i == HP || i == MP)
        {
            //pos = Vector2(PLATE_X + FIRST_OFFSET_X, PLATE_Y + FIRST_OFFSET_Y + ADD_OFFSET_Y * i);

            //// ステータス名
            //width = mStatusNameFont.GetWidth(i - 1);
            //mStatusNameFont.RenderSet(i - 1, pos); // k - 1 : 名前の分 

            //// 現在の値 (cur)
            //width = mFontValue.GetWidth(mHealthPlates[i].curHP);
            //pos.x += HealthPlate::CUR_OFFSET_X;
            //center.x = width;
            //mFontValue.RenderSet(mHealthPlates[i].curHP, pos, center);

            //// 区切り (/)
            //pos.x += HealthPlate::DELIM_OFFSET_X;
            //mDelimFont.RenderSet(0, pos);

            //// 最大の値 (max)
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
