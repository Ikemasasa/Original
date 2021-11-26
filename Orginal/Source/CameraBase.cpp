#include "CameraBase.h"

#include "lib/Framework.h"
#include "lib/Random.h"

#include "Player.h"
#include "Define.h"

CameraBase::CameraBase()
{

}

void CameraBase::Initialize(const Vector3& target, float distance)
{
    // 引数のパラメータからメンバを設定

    // ビュー関係の設定
    mTarget = target;
    mPos = CalcPosFromAngle(distance);
    UpdateView();

    // プロジェクション関係の設定(数値は適当)
    mFov = DirectX::XMConvertToRadians(45.0f);
    mAspect = Define::SCREEN_WIDTH / Define::SCREEN_HEIGHT;
    mNear = 1.0f;
    mFar = 1000.0f;
    SetPerspectiveMatrix(mFov, mAspect, mNear, mFar);
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

Vector3 CameraBase::CalcPosFromAngle(float distance)
{
    // x = sinf(theta) * cosf(phi) * r;
    // y = cosf(theta) * r;
    // z = sinf(theta) * sinf(phi) * r;

    Vector3 ret = {};
    ret.x = mTarget.x + sinf(mZenithAngle) * cosf(mAzimuthAngle) * distance;
    ret.y = mTarget.y + cosf(mZenithAngle) * distance;
    ret.z = mTarget.z + sinf(mZenithAngle) * sinf(mAzimuthAngle) * distance; 
    return ret;
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