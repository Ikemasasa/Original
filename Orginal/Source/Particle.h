#pragma once

#include "lib/Vector.h"

class Particle
{
	struct Vertex
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 tex;
		Vector4 color;
		Vector4 param; // x:��] yzw:�A�j��
	};

	float mLifeTime; // ��������
	float mDuration; // �G�~�b�^�[�̗L������
	float RateOverTime; // �� / �b(1�b�Ԃɉ��o����)
	float EmitCount;

	Vector3 EmitCenter; // �������S
	float mEmitRadius;  // �������a
};