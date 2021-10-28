#pragma once
#include <memory>

#include "lib/Matrix.h"
#include "lib/Vector.h"

class Character;
class BattleCharacter;
class BattleCharacterManager;

class CameraBase
{
protected:
	Vector3 mPos;
	Vector3 mTarget;
	Vector3 mAngle;
	Matrix mView	   = {};
	Matrix mProjection = {};
	float mFov = 0.0f;
	float mAspect = 0.0f;
	float mNear = 0.0f;
	float mFar = 0.0f;

	Vector3 mForward;
	Vector3 mRight;

	bool mIsShake = false;
	float mShakeDuration = 0.0f;
	float mShakeStrength = 0.0f;
	float mShakeElapsed = 0.0f;

	float mDistFromTargetY;

	void UpdateView();
	void UpdateShake(float elapsedTime);
public:
	CameraBase();
	virtual ~CameraBase() = default;

	virtual void Initialize(const Character* target);
	virtual void Update(const Character* target) {};
	virtual void Update(const BattleCharacter* commandChara, const BattleCharacterManager* bcm) {};

	void Shake(float duration, float strength);

	void SetOrthoMatrix(float w, float h, float znear, float zfar);					//	���s���e�s��ݒ�֐�
	void SetPerspectiveMatrix(float fov, float aspect, float znear, float zfar);	//	�������e�s��ݒ�֐�
	Matrix GetViewMatrix() const { return mView; }				//	�r���[�s��擾�֐�
	Matrix GetProjectionMatrix() const { return mProjection; }	//	���e�s��擾�֐�

	Vector3 GetForward() const { return mForward; }
	Vector3 GetRight() const { return mRight; }
	Vector3 GetAngle() const  { return mAngle; }
	Vector3 GetPos() const { return mPos; }
};