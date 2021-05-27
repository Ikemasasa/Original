#pragma once
#include <memory>

#include "lib/Vector.h"

class Actor;

class CameraBase
{
protected:
	Vector3 mPos;
	Vector3 mTarget;
	Vector3 mAngle;
	DirectX::XMFLOAT4X4 mView = {};
	DirectX::XMFLOAT4X4 mProjection = {};

	Vector3 mFrontVector;
	Vector3 mRightVector;

	float mDistFromTargetY;

	void UpdateView();
public:
	CameraBase();
	virtual ~CameraBase() = default;

	virtual void Update(const std::shared_ptr<Actor>& target) {};

	DirectX::XMFLOAT4X4& SetOrthographicMatrix(float w, float h, float znear, float zfar);			//	���s���e�s��ݒ�֐�
	DirectX::XMFLOAT4X4& SetPerspectiveMatrix(float fov, float aspect, float znear, float zfar);	//	�������e�s��ݒ�֐�
	DirectX::XMFLOAT4X4 GetViewMatrix() const { return mView; }										//	�r���[�s��擾�֐�
	DirectX::XMFLOAT4X4 GetProjectionMatrix() const { return mProjection; }		//	���e�s��擾�֐�

	Vector3 GetFrontVector() const { return mFrontVector; }
	Vector3 GetRightVector() const { return mRightVector; }
	Vector3 GetAngle() const  { return mAngle; }
	Vector3 GetPos() const { return mPos; }
};