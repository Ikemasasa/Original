#include "CameraBase.h"

#include "lib/Framework.h"
#include "lib/Random.h"

#include "Player.h"
#include "Define.h"

CameraBase::CameraBase()
{
    mTarget = Vector3::ZERO;
    mPos = Vector3(0, 5, 15);
    mAngle = Vector3::ZERO;

    mForward = Vector3::ZERO;
    mRight = Vector3::ZERO;
    mDistFromTargetY = 0.0f;

    mFov = DirectX::XMConvertToRadians(45.0f);
    mAspect = Define::SCREEN_WIDTH / Define::SCREEN_HEIGHT;
    mNear = 1.0f;
    mFar = 1000.0f;
    SetPerspectiveMatrix(mFov, mAspect, mNear, mFar);
    UpdateView();
}

void CameraBase::Initialize(const Character* target)
{
    mTarget = target->GetPos();
    
    // targetの後ろに就くように
    float angle = target->GetAngle().y;
    Vector3 behind(-sinf(angle), 0.0f, -cosf(angle));
    mPos = mTarget + behind;

    mAngle.y = target->GetAngle().y - Define::PI;

    mForward = Vector3::ZERO;
    mRight = Vector3::ZERO;
    mDistFromTargetY = 6.5f;
    UpdateView();
}

void CameraBase::UpdateView()
{
    // ビュー行列更新
    mView.LookAtLH(mPos, mTarget);

    mForward = -Vector3(mView._13, mView._23, mView._33);
    mRight = Vector3(mView._11, mView._21, mView._31);
}

void CameraBase::UpdateShake(float elapsedTime)
{
    if (mIsShake)
    {
        Vector3 right = mRight;
        Vector3 up = Vector3(mView._21, mView._22, mView._23);

        Vector3 horizontal = right * Random::RandomRangef(-1.0f, 1.0f) * mShakeStrength;
        Vector3 vertical = up * Random::RandomRangef(-1.0f, 1.0f) * mShakeStrength;
        
        mPos += horizontal + vertical;

        // タイマー
        mShakeElapsed += elapsedTime;
        if (mShakeElapsed >= mShakeDuration)
        {
            mIsShake = false;
        }
    }
}

void CameraBase::Shake(float duration, float strength)
{
    mIsShake = true;
    mShakeDuration = duration;
    mShakeStrength = strength;
    mShakeElapsed = 0.0f;
}

void CameraBase::SetOrthoMatrix(float w, float h, float znear, float zfar)
{
    mProjection.Ortho(w, h, znear, zfar);
}

void CameraBase::SetPerspectiveMatrix(float fov, float aspect, float znear, float zfar)
{
    mProjection.PerspectiveFov(fov, aspect, znear, zfar);
}