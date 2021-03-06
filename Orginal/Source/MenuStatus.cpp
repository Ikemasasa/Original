#include "MenuStatus.h"

#include "lib/Input.h"

#include "Define.h"
#include "KeyGuide.h"
#include "Player.h"
#include "PlayerManager.h"
#include "StatusData.h"

void MenuStatus::Initialize(const PlayerManager* plm)
{
    // 画像読み込み
    mStatusPlate = std::make_unique<Sprite>(L"Data/Image/Menu/status_board.png");

    // フォントクラス初期化
    mFont.Initialize();

    // キャラクタ―選択クラス初期化
    mCharacterSelect.Initialize(plm);
}

MenuBase::Select MenuStatus::Update(PlayerManager* plm)
{
    // 前の画面に戻す
    if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return BACK;

    // キーガイド
    KeyGuide::Instance().Add(KeyGuide::B, L"戻る");

    // キャラ選択
    mCharacterSelect.Update();

    const int playerIndex = mCharacterSelect.GetIndex();
    int charaID = plm->GetPlayer(playerIndex)->GetCharaID();
    const Status status = StatusData::GetPLStatus(charaID);
    std::vector<int> curValue; // 現在のステータス
    std::vector<int> maxValue; // 最大のステータス
    curValue.emplace_back(status.GetHP());
    curValue.emplace_back(status.GetMP());
    curValue.emplace_back(status.GetAtk());
    curValue.emplace_back(status.GetDef());
    curValue.emplace_back(status.GetSpd());
    maxValue.emplace_back(status.GetMaxHP());
    maxValue.emplace_back(status.GetMaxMP());

    const wchar_t* statusName[] = {
        L"HP",
        L"MP",
        L"攻撃力",
        L"防御力",
        L"素早さ",
    };

    for (int i = 0; i < STATUS_NUM; ++i)
    {
        float width = 0.0f;
        Vector2 pos = {};
        Vector2 center = {};

        const int statusIndex = i - 1;// i - 1 : 名前の分 

        // かなりごり押し、ほかにいい方法あれば変える
        if (i == NAME)
        {
            pos = Vector2(BOARD_X + mStatusPlate->GetSize().x * 0.5f, BOARD_Y + NAME_OFFSET_Y);
            center = Vector2(0.5f, 0.0f);
            mFont.RenderSet(status.GetName().c_str(), pos, center, Define::FONT_COLOR);
        }
        else if (i == HP || i == MP)
        {
            pos = Vector2(BOARD_X + STATUS_OFFSET_X, BOARD_Y + STATUS_OFFSET_Y + STATUS_ADD_Y * (i - 1));

            // ステータス名
            mFont.RenderSet(statusName[statusIndex], pos, Vector2::ZERO, Define::FONT_COLOR);

            // 現在の値 (cur)
            pos.x += CUR_OFFSET_X;
            mFont.RenderSetValue(curValue[statusIndex], pos, center, Define::FONT_COLOR);

            // 区切り (/)
            pos.x += DELIM_OFFSET_X + mFont.GetWidthValue(curValue[statusIndex]);
            mFont.RenderSet(L"/", pos, center, Define::FONT_COLOR);

            // 最大の値 (max)
            pos.x += mFont.GetWidth(L"/") + DELIM_OFFSET_X;
            mFont.RenderSetValue(maxValue[statusIndex], pos, center, Define::FONT_COLOR);
        }
        else
        {
            pos = Vector2(BOARD_X + STATUS_OFFSET_X, BOARD_Y + STATUS_OFFSET_Y + STATUS_ADD_Y * (i - 1));
            
            // ステータス名
            mFont.RenderSet(statusName[statusIndex], pos, Vector2::ZERO, Define::FONT_COLOR);

            // 現在の値 (cur)
            pos.x += CUR_OFFSET_X;
            mFont.RenderSetValue(curValue[statusIndex], pos, center, Define::FONT_COLOR);
        }
    }

    return NONE;
}

void MenuStatus::Render()
{
    // 画像系描画
    {
        Vector2 pos(BOARD_X, BOARD_Y);
        Vector2 scale(1.0f, 1.0f);
        Vector2 texPos(0.0f, 0.0f);
        Vector2 size(mStatusPlate->GetSize());
        mStatusPlate->Render(pos, scale, texPos, size);
        mCharacterSelect.Render(pos + Vector2(CHARA_BOARD_OFFSET_X, 0.0f));
    }

    // フォント描画
    mFont.Render();
}

void MenuStatus::Release()
{
    mFont.Release();
}
