#pragma once
#include "lib/Vector.h"

/* ����
* �w�b�_�[�̃C���N���[�h��
* �P�C(cpp�̏ꍇ) �Ή�����w�b�_
* �Q�C<>�ŃC���N���[�h�ł������
* �R�C���색�C�u�����̂ւ���
* �S�C���̑�
* 
* �P�C�Q�C�R�C�S�̊Ԃɋ󔒂����邱��
*/



struct Define final
{
	static constexpr float SCREEN_WIDTH = 1280.0f;
	static constexpr float SCREEN_HEIGHT = 720.0f;

	static constexpr float FRAMERATE = 60.0f;
	static constexpr float PI = 3.1416f;
	static constexpr float ROOT2 = 1.1414214f;

	static constexpr float DAMAGE_SHAKE_DURATION = 0.2f;
	static constexpr float DAMAGE_SHAKE_STRENGTH = 0.1f;

	static const int ENVIRONMENT_TEXTURE_SLOT = 13;
	static const int SHADOWMAP_TEXTURE_SLOT = 14;
	static const int RAMP_TEXTURE_SLOT = 15;
	

	static const Vector4 FONT_COLOR;
	static const Vector4 FONT_BUFF_COLOR;
	static const Vector4 FONT_DEBUFF_COLOR;
};