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

bool Fade::SetSceneImage(float fadeSpeed, bool isWave)
{
    if (mType != NONE) return false;
    if (fadeSpeed <= 0.0f) return false;
    
    if (!mWaveShader && isWave)
    {
        // 初回のみ読み込み
        mWaveShader = std::make_unique<Shader>();
        mWaveShader->Load2D(L"Shaders/Renderer2D.fx", "VSMain", "PSWave");
        mWaveCB.Create(sizeof(CBForWave));
    }

    // フェード画像は後で作る
    mSpeed = -fadeSpeed;
    mType = FADE_IN;
    mAlpha = 1.0f;
    mIsWave = isWave;

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

        mIsWave = false;
        mSinFactor = 0.0f;
    }
}

void Fade::Render()
{
    if (!mFade && mType == FADE_IN)
    {
        // このタイミングで作成しないと前回のフレームのスクショが作成されるからここでやってる
        const wchar_t* filename = L"Data/Image/screen_fade.png";
        FRAMEWORK.GenerateScrshot(filename);
        mFade = std::make_unique<Sprite>(filename);
    }

    if (mFade)
    {
        if (mIsWave)
        {
            CBForWave cb;
            cb.sinFactor = mSinFactor;
            cb.WaveNum = WAVENUM;
            mWaveCB.Update(&cb);
            mWaveCB.Set(0);
            mFade->Render(mWaveShader.get(), Vector2(0.0f, 0.0f), Vector2::ONE, Vector2::ZERO, mFade->GetSize(), Vector2::ZERO, 0.0f, Vector4(1.0f, 1.0f, 1.0f, mAlpha));

            mSinFactor += ADD_SINFACTOR;
        }
        else
        {
            mFade->Render(Vector2(0.0f, 0.0f), Vector2::ONE, Vector2::ZERO, Vector2(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT), Vector2::ZERO, 0.0f, Vector4(1.0f, 1.0f, 1.0f, mAlpha));
        }
    }
}

bool Fade::CheckFadeState(Fade::FadeType type, float alpha)
{
    bool ret = false;
    if (type == FADE_OUT) ret = (mAlpha <= alpha);
    if (type == FADE_IN) ret = (mAlpha >= alpha);

    return ret;
}
