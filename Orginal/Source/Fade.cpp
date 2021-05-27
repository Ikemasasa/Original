#include "Fade.h"
#include "Define.h"

#include "lib/Framework.h"


bool Fade::Set(float fadeSpeed)
{
    if (mType != NONE) return false;
    if (fadeSpeed <= 0.0f) return false;

    mFade = std::make_unique<Texture>(L"./Data/image/fade.png");
    mSpeed = fadeSpeed;
    mType = FADE_IN_OUT;
    return true;
}

bool Fade::SetSceneImage(float fadeSpeed)
{
    if (mType != NONE) return false;
    if (fadeSpeed <= 0.0f) return false;
    
    // フェード画像は後で作る
    mSpeed = -fadeSpeed;
    mType = FADE_OUT;
    mAlpha = 1.0f;

    return true;
    
}

void Fade::Update()
{
    if (mType == NONE) return;

    switch (mType)
    {
    case FADE_IN_OUT: UpdateFadeInOut(); break;
    case FADE_OUT: UpdateFadeOut();      break;
    }
}

void Fade::UpdateFadeInOut()
{
    mAlpha += mSpeed;
    if (mAlpha > 1.0f)
    {
        mAlpha = 1.0f;
        mSpeed = -mSpeed;
    }
    if (mAlpha < 0.0f)
    {
        mAlpha = 0.0f;
        mSpeed = 0.0f;
        mFade.reset();
        mType = NONE;
    }
}

void Fade::UpdateFadeOut()
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
    if (!mFade && mType == FADE_OUT)
    {
        // このタイミングで作成しないと前回のフレームのスクショが作成されるからここでやってる
        const wchar_t* filename = L"Data/Image/ScrFade.png";
        FRAMEWORK.GenerateScrshot(filename);
        mFade = std::make_unique<Texture>(filename);
    }

    if (mFade) mFade->Render(Vector2(0.0f, 0.0f), Vector2::One(), Vector2::Zero(), Vector2(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT), Vector2::Zero(), 0.0f, Vector4(1.0f, 1.0f, 1.0f, mAlpha));
}
