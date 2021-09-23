#include "MenuSelect.h"

#include "lib/Math.h"
#include "lib/Sprite.h"
#include "lib/Input.h"

#include "CharacterHealth.h"
#include "Define.h"
#include "Fade.h"
#include "Player.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "StatusData.h"

void MenuSelect::Initialize(const PlayerManager* plm)
{
    mStrBoard = std::make_unique<Sprite>(L"Data/Image/Menu/str_board.png");
    mStrSelect = std::make_unique<Sprite>(L"Data/Image/Menu/str_select.png");

    mCharacterHealth = std::make_unique<CharacterHealth>();
    mCharacterHealth->Initialize(Vector2(HEALTH_BOARD_POS_X, BOARD_POS_Y));

    mFont.Initialize();
    
    // 開始演出登録
    for (int i = 0; i < SELECT_NUM; ++i)
    {
        Data data;
        data.pos.x = BOARD_POS_X;
        data.pos.y = BOARD_POS_Y + ((mStrBoard->GetSize().y + BOARD_OFFSET_Y) * i);
        data.moveX = MOVE_MAX;
        mDatas.push_back(data);
    }
}

MenuSelect::Select MenuSelect::Update(PlayerManager* plm)
{
    BeginAnimation();

    // CharacterHealth作成
    std::vector<Status> statusArray;
    for (size_t i = 0; i < plm->GetNum(); ++i)
    {
        int charaID = plm->GetPlayer(i)->GetCharaID();
        Status status = StatusData::GetPLStatus(charaID);
        statusArray.push_back(status);
    }
    mCharacterHealth->Update(statusArray);

    // selectIndex操作
    if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + SELECT_NUM - 1) % SELECT_NUM;
    if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + 1) % SELECT_NUM;

    if (Input::GetButtonTrigger(0, Input::BUTTON::A))
    {
        return (Select)mSelectIndex;
    }


    // メニュー画面からフィールドに戻す
    if (Input::GetButtonTrigger(0, Input::BUTTON::B))
    {
        if (Fade::GetInstance().SetSceneImage(Fade::SPEED_FAST))
        {
            Audio::SoundStop((int)Sound::CANCEL);
            Audio::SoundPlay((int)Sound::MENU_CLOSE);
            SceneManager::GetInstance().PopCurrentScene();
        }
    }

    return NONE;
}

void MenuSelect::Render()
{
    const wchar_t* str[SELECT_NUM] =
    {
        L"アイテム",
        L"装備品",
        L"ステータス"
    };

    // ボードを描画(ついでにフォントもセット
    const Vector2 scale(Vector2::ONE);
    const Vector2 texPos(Vector2::ZERO);
    for (size_t i = 0; i < mDatas.size(); ++i)
    {
        const Vector2 pos(mDatas[i].pos.x + mDatas[i].moveX, mDatas[i].pos.y);
        mStrBoard->Render(pos, scale, texPos, mStrBoard->GetSize());
        mFont.RenderSet(str[i], pos + Vector2(STR_OFFSET_X, STR_OFFSET_Y), Vector2::ZERO, Define::FONT_COLOR);

        // 選択画像を描画
        if (mSelectIndex == i)
        {
            mStrSelect->Render(pos, scale, texPos, mStrSelect->GetSize());
        }
    }

    mCharacterHealth->Render(false);

    mFont.Render();
}

void MenuSelect::Release()
{
    mFont.Release();
}

void MenuSelect::BeginAnimation()
{
    const float MOVE_ADD = -2.0f;
    const float NEXT_MOVE = MOVE_MAX - MOVE_MAX / 3.0f; // 次が動き出す値

    for (int i = 0; i < SELECT_NUM; ++i)
    {
        bool isAnim = false;

        if (i == 0) isAnim = true;
        else
        {
            if (mDatas[i - 1].moveX < NEXT_MOVE) isAnim = true;
        }

        if (isAnim) mDatas[i].moveX = Math::Max(mDatas[i].moveX + MOVE_ADD, 0.0f);
    }
}