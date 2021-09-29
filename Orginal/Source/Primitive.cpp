#include "Primitive.h"

#include "lib/Shader.h"

void Primitive::UpdateWorld()
{
	// �e��X�V�̌�ɌĂяo��

	// ���t���[���̃��[���h�s����v�Z
	mWorld.SRT(mScale, mAngle, mPos);
}

void Primitive::Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir, const Vector4& color)
{
	Matrix wvp = mWorld * view * proj;
	mPrim->Render(shader, wvp, mWorld, lightDir, color);
}
