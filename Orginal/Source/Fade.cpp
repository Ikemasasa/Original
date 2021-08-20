#include "Fade.h"
#include "Define.h"

#include "lib/Framework.h"
#include "lib/Sprite.h"


bool Fade::Set(float fadeSpeed)
{
    if (mType != NONE) return false;
    if (fadeSpeed <= 0.0f) return false;

    mFade = std::make_unique<Sprite>(L"./Data/image/fade.png");
    mSpeed = fadeSpeed;
    mType = FADE_OUT;
    return true;
}

bool Fade::SetSceneImage(float fadeSpeed)
{
    if (mType != NONE) return false;
    if (fadeSpeed <= 0.0f) return false;
    
    // �t�F�[�h�摜�͌�ō��
    mSpeed = -fadeSpeed;
    mType = FADE_IN;
    mAlpha = 1.0f;

    return true;
    
}

void Fade::Update()
{
    if (mType == NONE) return;

    switch (mType)
    {
    case FADE_OUT: UpdateFadeOut(); break;
    case FADE_IN: UpdateFadeIn();   break;
    }
}

void Fade::UpdateFadeOut()
{
    mAlpha += mSpeed;
    if (mAlpha > 1.0f)
    {
        mAlpha = 1.0f;
        mSpeed = -mSpeed;
        mType = FADE_IN;
    }
}

void Fade::UpdateFadeIn()
{
    mAlpha += mSpeed;
    if (mAlpha < 0.0f)
    {
        mAlpha = 0.0f;
        mSpeed = 0.0f;
        mFade.reset();
        mType = NONE;
    }
}

void Fade::Render()
{
    if (!mFade && mType == FADE_IN)
    {
        // ���̃^�C�~���O�ō쐬���Ȃ��ƑO��̃t���[���̃X�N�V�����쐬����邩�炱���ł���Ă�
        const wchar_t* filename = L"Data/Image/screen_fade.png";
        FRAMEWORK.GenerateScrshot(filename);
        mFade = std::make_unique<Sprite>(filename);
    }

    if (mFade) mFade->Render(Vector2(0.0f, 0.0f), Vector2::ONE, Vector2::ZERO, Vector2(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT), Vector2::ZERO, 0.0f, Vector4(1.0f, 1.0f, 1.0f, mAlpha));
}
