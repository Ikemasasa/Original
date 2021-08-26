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

	static const Vector4 FONT_COLOR;
	static const Vector4 FONT_BUFF_COLOR;
};