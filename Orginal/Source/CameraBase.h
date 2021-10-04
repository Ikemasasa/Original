#pragma once
#include <memory>

#include "lib/Matrix.h"
#include "lib/Vector.h"

class Character;

class CameraBase
{
protected:
	Vector3 mPos;
	Vector3 mTarget;
	Vector3 mAngle;
	Matrix mView	   = {};
	Matrix mProjection = {};

	Vector3 mForward;
	Vector3 mRight;

	float mDistFromTargetY;

	void UpdateView();
public:
	CameraBase();
	virtual ~CameraBase() = default;

	virtual void Initialize(const Character* target);
	virtual void Update(const Character* target) {};

	void SetOrthoMatrix(float w, float h, float znear, float zfar);					//	���s���e�s��ݒ�֐�
	void SetPerspectiveMatrix(float fov, float aspect, float znear, float zfar);	//	�������e�s��ݒ�֐�
	Matrix GetViewMatrix() const { return mView; }				//	�r���[�s��擾�֐�
	Matrix GetProjectionMatrix() const { return mProjection; }	//	���e�s��擾�֐�

	Vector3 GetForward() const { return mForward; }
	Vector3 GetRight() const { return mRight; }
	Vector3 GetAngle() const  { return mAngle; }
	Vector3 GetPos() const { return mPos; }
};