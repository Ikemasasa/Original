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
    mStatusPlate = std::make_unique<Sprite>(L"Data/Image/Menu/status_board.png");
    mFont.Initialize();
    mFontValue.Initialize();
    mCharacterSelect.Initialize(plm);
}

MenuBase::Select MenuStatus::Update(PlayerManager* plm)
{
    // 前の画面に戻す
    if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return BACK;

    // 決定音を鳴らさない
    AUDIO.SoundStop((int)Sound::SELECT);

    mCharacterSelect.Update();

    const int playerIndex = mCharacterSelect.GetIndex();
    int charaID = plm->GetPlayer(playerIndex)->GetCharaID();
    const Status status = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(charaID);
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
            width = mFont.GetWidth(status.GetName().c_str());
            pos = Vector2(PLATE_X + mStatusPlate->GetSize().x * 0.5f, PLATE_Y + NAME_OFFSET_Y);
            center = Vector2(width * 0.5f, 0.0f);
            mFont.RenderSet(status.GetName().c_str(), pos, center, Define::FONT_COLOR);
        }
        else if (i == HP || i == MP)
        {
            pos = Vector2(PLATE_X + STATUS_OFFSET_X, PLATE_Y + STATUS_OFFSET_Y + STATUS_ADD_Y * (i - 1));

            // ステータス名
            mFont.RenderSet(statusName[statusIndex], pos, Vector2::ZERO, Define::FONT_COLOR);

            // 現在の値 (cur)
            pos.x += CUR_OFFSET_X;
            mFontValue.RenderSet(curValue[statusIndex], pos, center, Define::FONT_COLOR);

            // 区切り (/)
            pos.x += DELIM_OFFSET_X + mFontValue.GetWidth(curValue[statusIndex]);
            mFont.RenderSet(L"/", pos, Vector2::ZERO, Define::FONT_COLOR);

            // 最大の値 (max)
            pos.x += mFont.GetWidth(L"/") + DELIM_OFFSET_X;
            mFontValue.RenderSet(maxValue[statusIndex], pos, center, Define::FONT_COLOR);
        }
        else
        {
            pos = Vector2(PLATE_X + STATUS_OFFSET_X, PLATE_Y + STATUS_OFFSET_Y + STATUS_ADD_Y * (i - 1));
            
            // ステータス名
            mFont.RenderSet(statusName[statusIndex], pos, Vector2::ZERO, Define::FONT_COLOR);

            // 現在の値 (cur)
            pos.x += CUR_OFFSET_X;
            mFontValue.RenderSet(curValue[statusIndex], pos, center, Define::FONT_COLOR);
        }
    }

    return NONE;
}

void MenuStatus::Render()
{
    // 画像系描画
    {
        Vector2 pos(PLATE_X, PLATE_Y);
        Vector2 scale(1.0f, 1.0f);
        Vector2 texPos(0.0f, 0.0f);
        Vector2 size(mStatusPlate->GetSize());
        mStatusPlate->Render(pos, scale, texPos, size);
        mCharacterSelect.Render(pos);
    }

    // フォント描画
    mFont.Render();
    mFontValue.Render();
}

void MenuStatus::Release()
{
    mFont.Release();
    mFontValue.Release();
}
