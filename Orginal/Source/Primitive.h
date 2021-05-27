#pragma once
#include <memory>

#include "lib/Vector.h"
#include "lib/GeometricPrimitive.h"


class Primitive
{
protected:

	std::unique_ptr<GeometricPrimitive> mGP;
	
	DirectX::XMFLOAT4X4 mWorld = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	Vector3 mPos   = { 0,0,0 };
	Vector3 mScale = { 1,1,1 };
	Vector3 mAngle = { 0,0,0 };

public:
	Primitive() = default;
	~Primitive() = default;

	void UpdateWorld();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDirection, const DirectX::XMFLOAT4& color);

	void SetPos(const Vector3& v) { mPos = v; }
	void SetScale(const Vector3& v) { mScale = v; }
	void SetAngle(const Vector3& v) { mAngle = v; }
};

class Cube : public Primitive
{
public:
	Cube(const Vector3& min, const Vector3& max) { mGP = std::make_unique<GeometricCube>(min, max); }
};

class Sphere : public Primitive
{
public:
	Sphere() { mGP = std::make_unique<GeometricSphere>(); }
};

class Cylinder : public Primitive
{
public:
	Cylinder() { mGP = std::make_unique<GeometricCylinder>(); }
};

//class Capsule : public Primitive
//{
//public:
//	Capsule(const Vector3& cTop, const Vector3& cBottom) { mGP = std::make_unique<GeometricCapsule>(cTop, cBottom); }
//};

