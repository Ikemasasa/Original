#pragma once
#include "CameraBase.h"

class CameraTitle : public CameraBase
{
	static constexpr float POS_Y = 2.5f; // �J������Y���W(�K��

public:

	// �X�V�֐�(�I�[�o�[���C�h)
	void Update(const Character* chara) override;
};