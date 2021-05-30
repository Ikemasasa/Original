#include "MenuSelect.h"

#include "lib/Math.h"
#include "lib/Texture.h"
#include "lib/Input.h"

MenuSelect::MenuSelect()
{
    mSelectBar = std::make_unique<Texture>(L"Data/Image/Menu/plate_select.png");
    mBar = std::make_unique<Texture>(L"Data/Image/Menu/str_plate.png");
    
    const int FONT_SIZE = 64;
    const int FONT_WEIGHT = 64;
    mFont.Initialize(FONT_SIZE, FONT_WEIGHT);

    {
        std::wstring str[SELECT_NUM] =
        {
            L"アイテム",
            L"ステータス"
        };

        for (int i = 0; i < SELECT_NUM; ++i)
        {
            mFont.Add(str[i].c_str());
            Add(mBar.get(), str[i].c_str());
        }

    }
}

bool MenuSelect::Add(Texture* plate, const wchar_t* str)
{
    Data data;
    data.plate = plate;
    wcscpy_s(data.str, STR_MAX, str);
    data.pos.x = PLATE_OFFSET_X;
    data.pos.y = PLATE_FIRST_OFFSET_Y + (PLATE_OFFSET_Y * mDatas.size()) + (plate->GetSize().y * mDatas.size());
    data.moveX = MOVE_MAX;


    mDatas.emplace_back(data);
    return true;
}

void MenuSelect::BeginAnimation()
{
    const float MOVE_ADD = 2.0f;
    const float NEXT_MOVE = MOVE_MAX - MOVE_MAX / 5.0f; // 次が動き出す値

    for (int i = 0; i < SELECT_NUM; ++i)
    {
        bool isAnim = false;

        if (i == 0) isAnim = true;
        else
        {
            if (mDatas[i - 1].moveX > NEXT_MOVE) isAnim = true;
        }

        if(isAnim) mDatas[i].moveX = Math::Min(mDatas[i].moveX + MOVE_ADD, 0.0f);
    }

}

MenuSelect::Select MenuSelect::Update(PlayerManager* plm)
{
    BeginAnimation();

    // selectIndex操作
    if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + SELECT_NUM - 1) % SELECT_NUM;
    if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + 1) % SELECT_NUM;

    if (Input::GetButtonTrigger(0, Input::BUTTON::A))
    {
        return (Select)mSelectIndex;
    }

    return MENU_SELECT;
}

void MenuSelect::Render()
{
    if (!mDatas.empty())
    {
        const Vector2 pos(mDatas[mSelectIndex].pos.x + mDatas[mSelectIndex].moveX + mSelectBar->GetSize().x / 2.0f, mDatas[mSelectIndex].pos.y);
        const Vector2 scale(1.25f, 1.0f);
        const Vector2 center(mSelectBar->GetSize().x / 2.0f, 0.0f);
        const Vector4 color(0.5f, 0.8f, 0.95f, 0.8f);
        mSelectBar->Render(pos, scale, Vector2::Zero(), mSelectBar->GetSize(), center, 0.0f, color);
    }

    int i = 0;
    for (auto& data : mDatas)
    {
        const Vector2 strOffset(data.pos.x + data.moveX + STR_OFFSET_X, data.pos.y + STR_OFFSET_Y);
        data.plate->Render(data.pos + Vector2(data.moveX, 0.0f), Vector2::One(), Vector2::Zero(), data.plate->GetSize());
        mFont.RenderSet(i, strOffset, Vector2::Zero(), Vector2::One(), Vector4::One());
        ++i;
    }

    mFont.Render();
}

void MenuSelect::Release()
{
    mFont.Release();
}

