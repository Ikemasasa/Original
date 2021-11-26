#include "MenuSelect.h"

#include "lib/Math.h"
#include "lib/Sprite.h"
#include "lib/Input.h"

#include "Define.h"
#include "Fade.h"
#include "KeyGuide.h"
#include "Player.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "StatusData.h"
#include "Sound.h"

void MenuSelect::Initialize(const PlayerManager* plm)
{
    // �摜�ǂݍ���
    mStrBoard = std::make_unique<Sprite>(L"Data/Image/Menu/str_board.png");
    mStrSelect = std::make_unique<Sprite>(L"Data/Image/Menu/str_select.png");

    // �w���X�Q�[�W�N���X�쐬
    mHealthGauge.resize(plm->GetNum());
    for (auto& it : mHealthGauge) it.Initialize();

    // �t�H���g�N���X������
    mFont.Initialize();
    
    // �J�n���o�o�^
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

    // �L�[�K�C�h
    KeyGuide::Instance().Add(KeyGuide::A, L"����");
    KeyGuide::Instance().Add(KeyGuide::B, L"�߂�");
    KeyGuide::Key move[] = { KeyGuide::UP, KeyGuide::DOWN };
    KeyGuide::Instance().Add(move, 2, L"�J�[�\���ړ�");

    // HealthGauge�X�e�[�^�X�ݒ�
    for (size_t i = 0; i < plm->GetNum(); ++i)
    {
        // �L�����N�^ID���擾
        int charaID = plm->GetPlayer(i)->GetCharaID();

        // ���W�Z�o
        float x = HEALTH_BOARD_POS_X;
        float y = HEALTH_BOARD_POS_Y + mHealthGauge[i].GetHeight() * i;
        Vector2 pos(x, y);

        // �ݒ�
        mHealthGauge[i].Set(StatusData::GetPLStatus(charaID), pos, HealthGauge::LEFTTOP);
    }

    // selectIndex����
    if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + SELECT_NUM - 1) % SELECT_NUM;
    if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + 1) % SELECT_NUM;

    if (Input::GetButtonTrigger(0, Input::BUTTON::A))
    {
        return (Select)mSelectIndex;
    }


    // ���j���[��ʂ���t�B�[���h�ɖ߂�
    if (Input::GetButtonTrigger(0, Input::BUTTON::B))
    {
        if (Fade::GetInstance().SetSceneImage(Fade::SPEED_FAST))
        {
            Sound::Stop(Sound::CANCEL);
            Sound::Play(Sound::MENU_CLOSE);
            SceneManager::GetInstance().PopCurrentScene();

        }
    }
    return NONE;
}

void MenuSelect::Render()
{
    const wchar_t* str[SELECT_NUM] =
    {
        L"�A�C�e��",
        L"�����i",
        L"�X�e�[�^�X"
    };

    // �{�[�h��`��(���łɃt�H���g���Z�b�g
    const Vector2 scale(Vector2::ONE);
    const Vector2 texPos(Vector2::ZERO);
    for (size_t i = 0; i < mDatas.size(); ++i)
    {
        const Vector2 pos(mDatas[i].pos.x + mDatas[i].moveX, mDatas[i].pos.y);
        mStrBoard->Render(pos, scale, texPos, mStrBoard->GetSize());
        mFont.RenderSet(str[i], pos + Vector2(STR_OFFSET_X, STR_OFFSET_Y), Vector2::ZERO, Define::FONT_COLOR);

        // �I���摜��`��
        if (mSelectIndex == i)
        {
            mStrSelect->Render(pos, scale, texPos, mStrSelect->GetSize());
        }
    }

    for (auto& gauge : mHealthGauge)
    {
        gauge.Render();
    }

    mFont.Render();
}

void MenuSelect::Release()
{
    mFont.Release();
}

void MenuSelect::BeginAnimation()
{
    const float MOVE_ADD = -2.0f;
    const float NEXT_MOVE = MOVE_MAX - MOVE_MAX / 3.0f; // ���������o���l

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