#pragma once
#include <stack>
#include <memory>

#include "lib/Vector.h"
#include "lib/Matrix.h"

#include "CameraBase.h"

// �O���錾
class Character;
class BattleCharacter;
class BattleCharacterManager;


/*
	�J�����}�l�[�W���[
	�J�����̏���F�X�ȂƂ��납��Ƃ邽�߂ɍ����
	�Ȃ̂�Singleton�N���X��ʂ�����
*/
class CameraManager
{
	// �ϐ�

	// SceneManager��stack�g���Ă邩��J�������ꉞ
	std::stack<std::unique_ptr<CameraBase>> mMainCamera;

public: // �֐�

	// �R���X�g���N�^
	CameraManager();

	// �f�X�g���N�^
	~CameraManager();
	
	// �J�������}�l�[�W���ɒǉ�
	void Push(std::unique_ptr<CameraBase> camera) { mMainCamera.emplace(camera.release()); };

	// �J�������}�l�[�W������폜
	void Pop() { if(!mMainCamera.empty()) mMainCamera.pop(); }

	// �������֐�
	void Initialize(const Vector3& target, float distance = 5.0f);

	// �X�V�֐�
	void Update(const Vector3& target);
	void Update(const Character* target);
	void Update(const BattleCharacter* target, const BattleCharacterManager* bcm);

	// �h�炵�̃p�����[�^�ݒ�֐�
	void Shake(float duration, float strength);

	// �Q�b�^�[
	Matrix GetView() const;		  // �r���[�s��擾
	Matrix GetProj() const;		  // �v���W�F�N�V�����s��擾	
	Vector3 GetPos() const;		  // ���W�擾
	float GetZenithAngle() const;  // �V���p(�c)�擾
	float GetAzimuthAngle() const; // ���ʊp(��)�擾
	Vector3 GetForward() const;	  // �O���x�N�g���擾
	Vector3 GetForwardXZ() const;  // XZ���ʂ̑O���x�N�g���擾
	Vector3 GetRight() const;      // �E���x�N�g���擾
	Vector3 GetRightXZ() const;    // XZ���ʂ̉E���x�N�g���擾
};