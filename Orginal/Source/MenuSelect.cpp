#include "MenuSelect.h"

#include "lib/Math.h"
#include "lib/Texture.h"
#include "lib/Input.h"

MenuSelect::MenuSelect()
{
    mSelectBar = std::make_unique<Texture>(L"Data/Image/Menu/BarSelect.png");
    mBar = std::make_unique<Texture>(L"Data/Image/Menu/Bar.png");
    
    const int FONT_SIZE = 64;
    const int FONT_WEIGHT = 64;
    mFont.Initialize(FONT_SIZE, FONT_WEIGHT);

    {
        std::wstring str[SELECT_NUM] =
        {
            L"アイテム",
            L"ステータス"
        };

        mSelectStrs.resize(SELECT_NUM);
        for (int i = 0; i < SELECT_NUM; ++i)
        {
            mSelectStrs[i] = str[i];
            mFont.Add(str[i].c_str());
            Add(mBar.get(), str[i].c_str());
        }

    }
}

bool MenuSelect::Add(Texture* bar, const wchar_t* str)
{
    Data data;
    data.bar = bar;
    wcscpy_s(data.str, STR_MAX, str);
    data.pos.x = BAR_OFFSET_X;
    data.pos.y = BAR_FIRST_OFFSET_Y + (BAR_OFFSET_Y * mDatas.size()) + (bar->GetSize().y * mDatas.size());
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

MenuSelect::Select MenuSelect::Update()
{
    BeginAnimation();

    // selectIndex操作
    if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + SELECT_NUM - 1) % SELECT_NUM;
    if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + 1) % SELECT_NUM;

    if (Input::GetButtonTrigger(0, Input::BUTTON::A))
    {
        return (Select)mSelectIndex;
    }

    return SELECT_MENU;
}

void MenuSelect::Render()
{
    if (!mDatas.empty())
    {
        const Vector2 pos(mDatas[mSelectIndex].pos.x + mDatas[mSelectIndex].moveX + SELECT_OFFSET_X, mDatas[mSelectIndex].pos.y);
        mSelectBar->Render(pos, Vector2::One(), Vector2::Zero(), mSelectBar->GetSize());
    }

    int i = 0;
    for (auto& data : mDatas)
    {
        const Vector2 strOffset(data.pos.x + data.moveX + STR_OFFSET_X, data.pos.y + STR_OFFSET_Y);
        data.bar->Render(data.pos + Vector2(data.moveX, 0.0f), Vector2::One(), Vector2::Zero(), data.bar->GetSize());
        mFont.RenderSet(mSelectStrs[i].c_str(), strOffset, Vector2::Zero(), Vector2::One(), Vector4::One());
        ++i;
    }

    mFont.Render();
}

void MenuSelect::Release()
{
    mFont.Release();
}

