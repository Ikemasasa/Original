#include "CameraBase.h"

#include "lib/Framework.h"

#include "Player.h"
#include "Define.h"

CameraBase::CameraBase()
{
    mTarget = Vector3(0.0f, 0.0f, 0.0f);
    mPos = mTarget + Vector3(.0f, 4.0f, -15.0f);
    mAngle = Vector3(0.0f, 0.0f, 0.0f);
    mFrontVector = Vector3::ZERO;
    mRightVector = Vector3::ZERO;
    mDistFromTargetY = 0.0f;

    constexpr float	fov = DirectX::XMConvertToRadians(45.0f);
    float	aspect = Define::SCREEN_WIDTH / Define::SCREEN_HEIGHT;
    SetPerspectiveMatrix(fov, aspect, 0.1f, 1000.0f);
    UpdateView();
}

void CameraBase::UpdateView()
{
    DirectX::XMVECTOR p = DirectX::XMVectorSet(mPos.x, mPos.y, mPos.z, 0.0f);
    DirectX::XMVECTOR t = DirectX::XMVectorSet(mTarget.x, mTarget.y, mTarget.z, 0.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    DirectX::XMStoreFloat4x4(&mView, DirectX::XMMatrixLookAtLH(p, t, up));

    mFrontVector = Vector3(mView._13, mView._23, mView._33);
    mRightVector = Vector3(mView._11, mView._21, mView._31);
}



DirectX::XMFLOAT4X4& CameraBase::SetOrthographicMatrix(float w, float h, float znear, float zfar)
{
    DirectX::XMStoreFloat4x4(&mProjection, DirectX::XMMatrixOrthographicLH(w, h, znear, zfar));
    return mProjection;
}

DirectX::XMFLOAT4X4& CameraBase::SetPerspectiveMatrix(float fov, float aspect, float znear, float zfar)
{
    DirectX::XMStoreFloat4x4(&mProjection, DirectX::XMMatrixPerspectiveFovLH(fov, aspect, znear, zfar));
    return mProjection;
}