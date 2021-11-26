#pragma once
#include <memory>

#include "CameraBase.h"

// �O�l�̎��_�̃J����
class CameraTPS : public CameraBase
{
	// �萔
	static constexpr float LERP_FACTOR = 0.5f;	// ���`�⊮�̌W��
	static constexpr float DISTANCE = 15.0f;	// �^�[�Q�b�g����J�����̍ő勗��
	static constexpr float SPEED_X = 1.6f;		// �������̈ړ����x
	static constexpr float SPEED_Y = 1.0f;		// �c�����̈ړ����x

public:// �֐�

	// �R���X�g���N�^
	CameraTPS();

	// �f�X�g���N�^
	~CameraTPS() = default;

	// �X�V�֐�(�I�[�o�[���C�h)
	void Update(const Vector3& target) override;
};
