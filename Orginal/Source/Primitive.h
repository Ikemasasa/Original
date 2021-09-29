#pragma once
#include <memory>

#include "lib/Matrix.h"
#include "lib/Primitive3D.h"
#include "lib/Vector.h"

class Shader;

class Primitive
{
protected:
	std::unique_ptr<Primitive3D> mPrim;
	
	Matrix mWorld  = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	Vector3 mPos   = { 0, 0, 0 };
	Vector3 mScale = { 1, 1, 1 };
	Vector3 mAngle = { 0, 0, 0 };

public:
	Primitive() = default;
	~Primitive() = default;

	void UpdateWorld();
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir, const Vector4& color);

	void SetPos(const Vector3& v) { mPos = v; }
	void SetScale(const Vector3& v) { mScale = v; }
	void SetAngle(const Vector3& v) { mAngle = v; }
};