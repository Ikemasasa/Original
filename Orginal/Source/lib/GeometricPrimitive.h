#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include <wrl.h>
#include "CollisionStructs.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Shader.h"

class GeometricPrimitive
{
protected:
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
	};
	struct Face
	{
		Vector3 vertex[3];
		int materialIndex = 1;
	};
	struct Cbuffer
	{
		DirectX::XMFLOAT4X4 wvp;
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 lightDir;
		DirectX::XMFLOAT4 materialColor;
	};


	D3D_PRIMITIVE_TOPOLOGY mTopology;
	std::vector<Vertex> mVertices;
	std::vector<WORD> mIndices;
	std::vector<Face> mFaces;
	// Comptr
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstBuffer;
	Shader mShader;

	void Initialize();
	virtual HRESULT SetVertexBuffer(ID3D11Device* device) = 0;
	virtual HRESULT SetIndexBuffer(ID3D11Device* device) = 0;
	HRESULT CreateVertexBuffer(ID3D11Device* device);
	HRESULT CreateIndexBuffer(ID3D11Device* device);
public:
	// 当たり判定用(ローカル)
	AABB mAABB;

	GeometricPrimitive();
	virtual ~GeometricPrimitive();
	void Render(
		const DirectX::XMFLOAT4X4& wvp, // ワールド・ビュー・プロジェクション合成行列
		const DirectX::XMFLOAT4X4& world, // ワールド変換行列
		const DirectX::XMFLOAT4& lightDirection,	 // ライト進行方向
		const DirectX::XMFLOAT4& color    // 材質色
		);

	int RayCast(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);
};

class GeometricCube : public GeometricPrimitive
{
	Vector3 mMin;
	Vector3 mMax;

public:
	GeometricCube(const Vector3& min, const Vector3& max) : GeometricPrimitive() { mMin = min; mMax = max; Initialize();  };
	~GeometricCube() = default;
	HRESULT SetVertexBuffer(ID3D11Device* device) override;
	HRESULT SetIndexBuffer(ID3D11Device* device) override;

	void SetMin(const Vector3& min) { mMin = min; }
	void SetMax(const Vector3& max) { mMax = max; }
};

class GeometricSphere : public GeometricPrimitive
{
private:
	static const int U_MAX = 30;
	static const int V_MAX = 15;

public:
	GeometricSphere() : GeometricPrimitive() { Initialize(); };
	~GeometricSphere() = default;
	HRESULT SetVertexBuffer(ID3D11Device* device) override;
	HRESULT SetIndexBuffer(ID3D11Device* device) override;
};

class GeometricCylinder : public GeometricPrimitive
{
private:
	static const int U_MAX = 25;
	static const int V_MAX = 2;

public:
	GeometricCylinder() : GeometricPrimitive() { Initialize(); }
	~GeometricCylinder() = default;
	HRESULT SetVertexBuffer(ID3D11Device* device) override;
	HRESULT SetIndexBuffer(ID3D11Device* device) override;
};

class GeometricCapsule: public GeometricPrimitive
{
private:
	static const int U_MAX = 16;
	static const int V_MAX = 8;

	Vector3 mCenterTop;
	Vector3 mCenterBottom;
	float mRadius;
public:
	GeometricCapsule(const Vector3& cTop, const Vector3& cBottom, const float radius) : GeometricPrimitive()
	{ 
		mCenterTop = cTop;
		mCenterBottom = cBottom; 
		mRadius = radius;

		Initialize(); 
	}
	~GeometricCapsule() = default;
	HRESULT SetVertexBuffer(ID3D11Device* device) override;
	HRESULT SetIndexBuffer(ID3D11Device* device) override;
};

