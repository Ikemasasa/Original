#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>
#include <wrl.h>

#include "CollisionStructs.h"
#include "Matrix.h"
#include "Vector.h"

class Shader;

class Primitive3D
{
protected:
	struct Vertex
	{
		Vector3 pos;
		Vector3 normal;
	};

	struct Cbuffer
	{
		Matrix wvp;
		Matrix world;
		Vector4 lightDir;
		Vector4 color;
	};

	D3D_PRIMITIVE_TOPOLOGY mTopology;
	std::vector<Vertex> mVertices;
	std::vector<WORD> mIndices;
	int mFaceNum;

	// TransformèÓïÒ
	Vector3 mPos;
	Vector3 mScale;
	Vector3 mRotate;
	Matrix mWorld;

	// Comptr
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstBuffer;
	std::unique_ptr<Shader> mShader;

	void CreateShader();
	void Initialize();
	virtual HRESULT SetVertexBuffer(ID3D11Device* device) = 0;
	virtual HRESULT SetIndexBuffer(ID3D11Device* device) = 0;
	HRESULT CreateVertexBuffer(ID3D11Device* device);
	HRESULT CreateIndexBuffer(ID3D11Device* device);
public:
	// ìñÇΩÇËîªíËóp(ÉçÅ[ÉJÉã)
	AABB mAABB;

	Primitive3D();
	virtual ~Primitive3D();
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir, const Vector4 color);
	void RenderWire(const Shader* shader, const Matrix& wvp, const Matrix& world, const Vector4& lightDir, const Vector4 color);
	int RayCast(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);

	// Transformä÷åW
	Vector3 GetPos() const { return mPos; }
	Vector3 GetScale() const { return mScale; }
	Vector3 GetRotate() const { return mRotate; }
	void SetPos(const Vector3& v)	 { mPos = v; }
	void SetScale(const Vector3& v)  { mScale = v; }
	void SetRotate(const Vector3& v) { mRotate = v; }
	void UpdateWorldMatrix() { mWorld.SRT(mScale, mRotate, mPos); }


};

class Cube : public Primitive3D
{
	Vector3 mMin;
	Vector3 mMax;

public:
	Cube(const Vector3& min, const Vector3& max) : Primitive3D() { mMin = min; mMax = max; Initialize();  };
	~Cube() = default;
	HRESULT SetVertexBuffer(ID3D11Device* device) override;
	HRESULT SetIndexBuffer(ID3D11Device* device) override;

	void SetMin(const Vector3& min) { mMin = min; }
	void SetMax(const Vector3& max) { mMax = max; }
};

class Sphere : public Primitive3D
{
private:
	static const int U_MAX = 30;
	static const int V_MAX = 15;
	float mRadius;
	int mSlices;
	int mStacks;

public:
	Sphere(float radius, int slices = 16, int stacks = 16) : Primitive3D(), mRadius(radius), mSlices(slices), mStacks(stacks){ Initialize(); };
	~Sphere() = default;
	HRESULT SetVertexBuffer(ID3D11Device* device) override;
	HRESULT SetIndexBuffer(ID3D11Device* device) override;
};

class Cylinder : public Primitive3D
{
private:
	static const int U_MAX = 25;
	static const int V_MAX = 2;

public:
	Cylinder() : Primitive3D() { Initialize(); }
	~Cylinder() = default;
	HRESULT SetVertexBuffer(ID3D11Device* device) override;
	HRESULT SetIndexBuffer(ID3D11Device* device) override;
};

class Capsule : public Primitive3D
{
private:
	static const int U_MAX = 16;
	static const int V_MAX = 8;

	Vector3 mCenterTop;
	Vector3 mCenterBottom;
	float mRadius;
public:
	Capsule(const Vector3& cTop, const Vector3& cBottom, const float radius) : Primitive3D()
	{ 
		mCenterTop = cTop;
		mCenterBottom = cBottom; 
		mRadius = radius;

		Initialize(); 
	}
	~Capsule() = default;
	HRESULT SetVertexBuffer(ID3D11Device* device) override;
	HRESULT SetIndexBuffer(ID3D11Device* device) override;
};

