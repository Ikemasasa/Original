#include "Primitive3D.h"

#include "Framework.h"
#include "Math.h"
#include "ResourceManager.h"
#include "Shader.h"

Primitive3D::Primitive3D()
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
	mConstBuffer = nullptr;
	mTopology = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	mVertices.clear();
	mIndices.clear();
}

Primitive3D::~Primitive3D()
{
	mVertices.clear();
	mIndices.clear();
}

void Primitive3D::Initialize()
{
	auto device = FRAMEWORK.GetDevice();

	HRESULT hr;

	hr = SetVertexBuffer(device);
	if (FAILED(hr)) return;
	
	hr = SetIndexBuffer(device);
	if (FAILED(hr)) return;


	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(Cbuffer);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bufferDesc, NULL, mConstBuffer.GetAddressOf());
	if (FAILED(hr)) return;
}

HRESULT Primitive3D::CreateVertexBuffer(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA subresourceData;
	ZeroMemory(&subresourceData, sizeof(subresourceData));
	subresourceData.pSysMem = mVertices.data();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(Vertex) * mVertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mVertexBuffer.GetAddressOf());
	if (FAILED(hr)) return hr;

	return hr;
}

HRESULT Primitive3D::CreateIndexBuffer(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA subresourceData;
	ZeroMemory(&subresourceData, sizeof(subresourceData));
	subresourceData.pSysMem = mIndices.data();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(WORD) * mIndices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mIndexBuffer.GetAddressOf());
	if (FAILED(hr)) return hr;
	
	return hr;
}


void Primitive3D::Render(const Shader* shader, const Matrix& wvp, const Matrix& world, const Vector4& lightDir, const Vector4 color)
{
	auto context = FRAMEWORK.GetContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	shader->Activate();

	context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(mTopology);
	
	Cbuffer cb = {};
	cb.wvp = wvp;
	cb.world = world;
	cb.lightDir = lightDir;
	cb.color = color;
	context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, &cb, 0, 0);
	context->VSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());

	context->DrawIndexed(mIndices.size(), 0, 0);

	context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

int Primitive3D::RayCast(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal)
{
	int ret = -1; // ヒットした面番号
	float minDist = FLT_MAX; // 一番近いヒット面までの距離

	// 高速化のため
	Vector3 vertex[3] = {};
	Vector3 line[3] = {};
	Vector3 normal;
	Vector3 v0;
	Vector3 cp;

	Vector3 work;
	Vector3 interior;
	size_t size = mFaces.size();
	for (size_t i = 0; i < size; ++i)
	{
		auto& face = mFaces[i];

		// 面頂点取得
		vertex[0] = face.vertex[0];
		vertex[1] = face.vertex[1];
		vertex[2] = face.vertex[2];

		// 3辺算出
		// line = vertex - vertex;
		line[0].x = vertex[1].x - vertex[0].x;
		line[0].y = vertex[1].y - vertex[0].y;
		line[0].z = vertex[1].z - vertex[0].z;

		line[1].x = vertex[2].x - vertex[1].x;
		line[1].y = vertex[2].y - vertex[1].y;
		line[1].z = vertex[2].z - vertex[1].z;

		line[2].x = vertex[0].x - vertex[2].x;
		line[2].y = vertex[0].y - vertex[2].y;
		line[2].z = vertex[0].z - vertex[2].z;

		// 外積による法線算出
		// normal = line[0].Cross(line[1]);
		normal.x = line[0].y * line[1].z - line[0].z * line[1].y;
		normal.y = line[0].z * line[1].x - line[0].x * line[1].z;
		normal.z = line[0].x * line[1].y - line[0].y * line[1].x;
		normal.Normalize(); // 正規化

		// 法線をレイ方向に射影(内積)
		// dot = -normal.Dot(velocity);
		float dot = -normal.x * velocity.x + -normal.y * velocity.y + -normal.z * velocity.z;
		if (dot <= 0) continue;


		// ポリゴンの1頂点からレイの開始地点のベクトル
		v0.x = vertex[0].x - pos.x;
		v0.y = vertex[0].y - pos.y;
		v0.z = vertex[0].z - pos.z;

		// ↑のベクトルとポリゴンの法線の内積(射影を出す)
		// float dot2 = -normal.Dot(v0);
		float dot2 = -normal.x * v0.x + -normal.y * v0.y + -normal.z * v0.z;

		// レイの長さ
		float len = dot2 / dot;
		if (len < 0 || len > minDist) continue;

		// 交点算出
		cp.x = pos.x + velocity.x * len;
		cp.y = pos.y + velocity.y * len;
		cp.z = pos.z + velocity.z * len;


		// 内点判定
		// 1つ目
		work.x = vertex[0].x - cp.x;
		work.y = vertex[0].y - cp.y;
		work.z = vertex[0].z - cp.z;
		interior.x = work.y * line[0].z - work.z * line[0].y;
		interior.y = work.z * line[0].x - work.x * line[0].z;
		interior.z = work.x * line[0].y - work.y * line[0].x;

		float d = normal.x * interior.x + normal.y * interior.y + normal.z * interior.z;
		if (d < 0) continue; // 内積の結果マイナスなら外側

		// 2つ目
		work.x = vertex[1].x - cp.x;
		work.y = vertex[1].y - cp.y;
		work.z = vertex[1].z - cp.z;
		interior.x = work.y * line[1].z - work.z * line[1].y;
		interior.y = work.z * line[1].x - work.x * line[1].z;
		interior.z = work.x * line[1].y - work.y * line[1].x;

		d = normal.x * interior.x + normal.y * interior.y + normal.z * interior.z;
		if (d < 0) continue; // 内積の結果マイナスなら外側

		// 3つ目
		work.x = vertex[2].x - cp.x;
		work.y = vertex[2].y - cp.y;
		work.z = vertex[2].z - cp.z;
		interior.x = work.y * line[2].z - work.z * line[2].y;
		interior.y = work.z * line[2].x - work.x * line[2].z;
		interior.z = work.x * line[2].y - work.y * line[2].x;

		d = normal.x * interior.x + normal.y * interior.y + normal.z * interior.z;
		if (d < 0) continue; // 内積の結果マイナスなら外側

		// nan チェック
		//if (isnan(DirectX::XMVectorGetX(cp))) continue;
		//if (isnan(DirectX::XMVectorGetY(cp))) continue;
		//if (isnan(DirectX::XMVectorGetZ(cp))) continue;

		// 情報保存
		*outPos = cp;		 // 交点
		*outNormal = normal; // 法線
		minDist = len;		 // 最短距離更新
		ret = i;			 // i番目のポリゴンにヒット
	}

	return ret;
}

/* memo 
	u = x,
	v = y,
*/

// geometric_cube
HRESULT Cube::SetVertexBuffer(ID3D11Device* device)
{
	//Vertex cubeVertices[] =
	//{
	//	{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
	//	{{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
	//	{{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}},
	//	{{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}},

	//	{{ 0.5f, -0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f}},
	//	{{ 0.5f,  0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f}},
	//	{{ 0.5f,  0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f}},
	//	{{ 0.5f, -0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f}},

	//	{{-0.5f, -0.5f, -0.5f} ,{0.0f, -1.0f, 0.0f}},
	//	{{-0.5f, -0.5f,  0.5f} ,{0.0f, -1.0f, 0.0f}},
	//	{{ 0.5f, -0.5f,  0.5f} ,{0.0f, -1.0f, 0.0f}},
	//	{{ 0.5f, -0.5f, -0.5f} ,{0.0f, -1.0f, 0.0f}},

	//	{{-0.5f,  0.5f,  0.5f} ,{0.0f,  1.0f, 0.0f}},
	//	{{-0.5f,  0.5f, -0.5f} ,{0.0f,  1.0f, 0.0f}},
	//	{{ 0.5f,  0.5f, -0.5f} ,{0.0f,  1.0f, 0.0f}},
	//	{{ 0.5f,  0.5f,  0.5f} ,{0.0f,  1.0f, 0.0f}},

	//	{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
	//	{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
	//	{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
	//	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},

	//	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}},
	//	{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}},
	//	{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}},
	//	{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}}
	//};

	mVertices.clear();

	Vertex cubeVertices[] =
	{
		{{mMin.x, mMax.y, mMax.z}, {-1.0f, 0.0f, 0.0f}},
		{{mMin.x, mMax.y, mMin.z}, {-1.0f, 0.0f, 0.0f}},
		{{mMin.x, mMin.y, mMax.z}, {-1.0f, 0.0f, 0.0f}},
		{{mMin.x, mMin.y, mMin.z}, {-1.0f, 0.0f, 0.0f}},

		{{mMax.x, mMax.y, mMin.z}, { 1.0f, 0.0f, 0.0f}},
		{{mMax.x, mMax.y, mMax.z}, { 1.0f, 0.0f, 0.0f}},
		{{mMax.x, mMin.y, mMin.z}, { 1.0f, 0.0f, 0.0f}},
		{{mMax.x, mMin.y, mMax.z}, { 1.0f, 0.0f, 0.0f}},

		{{mMin.x, mMin.y, mMin.z} ,{0.0f, -1.0f, 0.0f}},
		{{mMax.x, mMin.y, mMin.z} ,{0.0f, -1.0f, 0.0f}},
		{{mMin.x, mMin.y, mMax.z} ,{0.0f, -1.0f, 0.0f}},
		{{mMax.x, mMin.y, mMax.z} ,{0.0f, -1.0f, 0.0f}},

		{{mMin.x, mMax.y, mMax.z} ,{0.0f,  1.0f, 0.0f}},
		{{mMax.x, mMax.y, mMax.z} ,{0.0f,  1.0f, 0.0f}},
		{{mMin.x, mMax.y, mMin.z} ,{0.0f,  1.0f, 0.0f}},
		{{mMax.x, mMax.y, mMin.z} ,{0.0f,  1.0f, 0.0f}},

		{{mMin.x, mMax.y, mMin.z}, {0.0f, 0.0f, -1.0f}},
		{{mMax.x, mMax.y, mMin.z}, {0.0f, 0.0f, -1.0f}},
		{{mMin.x, mMin.y, mMin.z}, {0.0f, 0.0f, -1.0f}},
		{{mMax.x, mMin.y, mMin.z}, {0.0f, 0.0f, -1.0f}},

		{{mMax.x, mMax.y, mMax.z}, {0.0f, 0.0f,  1.0f}},
		{{mMin.x, mMax.y, mMax.z}, {0.0f, 0.0f,  1.0f}},
		{{mMax.x, mMin.y, mMax.z}, {0.0f, 0.0f,  1.0f}},
		{{mMin.x, mMin.y, mMax.z}, {0.0f, 0.0f,  1.0f}}
	};


	for (auto& v : cubeVertices)
	{
		mVertices.push_back(v);
		mAABB.min.x = Math::Min(mAABB.min.x, v.position.x);
		mAABB.min.y = Math::Min(mAABB.min.y, v.position.y);
		mAABB.min.z = Math::Min(mAABB.min.z, v.position.z);
		mAABB.max.x = Math::Max(mAABB.max.x, v.position.x);
		mAABB.max.y = Math::Max(mAABB.max.y, v.position.y);
		mAABB.max.z = Math::Max(mAABB.max.z, v.position.z);
	}

	mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	return CreateVertexBuffer(device);
}

HRESULT Cube::SetIndexBuffer(ID3D11Device* device)
{
	mIndices.clear();

	WORD cubeIndices[] =
	{
		 0,  2,  1,    1,  2,  3,
		 4,  6,  5,    5,  6,  7,
		 8, 10,  9,    9, 10, 11,
		12, 14, 13,   13, 14, 15,
		16, 18, 17,   17, 18, 19,
		20, 22, 21,   21, 22, 23,
	};

	Face f;
	for (auto& i : cubeIndices)
	{
		mIndices.push_back(i);
	}

	// RayCast用
	for (size_t i = 0; i < mIndices.size(); i += 3)
	{
		Face f;
		f.vertex[0] = mVertices[mIndices[i]].position;
		f.vertex[1] = mVertices[mIndices[i + 1]].position;
		f.vertex[2] = mVertices[mIndices[i + 2]].position;
		mFaces.push_back(f);
	}

	return CreateIndexBuffer(device);
}


// geometric_shpere
HRESULT Sphere::SetVertexBuffer(ID3D11Device* device)
{
	for (int v = 0; v <= V_MAX; ++v)
	{
		for (int u = 0; u < U_MAX; ++u)
		{
			double theta = DirectX::XMConvertToRadians(180.0f * v / V_MAX);
			double phi = DirectX::XMConvertToRadians(360.0f * u / U_MAX);
			FLOAT X = static_cast<FLOAT>(sin(theta) * cos(phi));
			FLOAT Y = static_cast<FLOAT>(cos(theta));
			FLOAT Z = static_cast<FLOAT>(sin(theta) * sin(phi));
			Vertex temp = {};
			temp.position = Vector3(X, Y, Z);
			temp.normal = Vector3(X, Y, Z);
			mVertices.push_back(temp);

			mAABB.min.x = Math::Min(mAABB.min.x, temp.position.x);
			mAABB.min.y = Math::Min(mAABB.min.y, temp.position.y);
			mAABB.min.z = Math::Min(mAABB.min.z, temp.position.z);
			mAABB.max.x = Math::Max(mAABB.max.x, temp.position.x);
			mAABB.max.y = Math::Max(mAABB.max.y, temp.position.y);
			mAABB.max.z = Math::Max(mAABB.max.z, temp.position.z);
		}
	}

	mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	return CreateVertexBuffer(device);
}

HRESULT Sphere::SetIndexBuffer(ID3D11Device* device)
{
	for (int v = 0; v < V_MAX; ++v)
	{
		for (int u = 0; u <= U_MAX; ++u)
		{
			if (u == U_MAX)
			{
				mIndices.push_back(v * U_MAX);
				mIndices.push_back((v + 1) * U_MAX);
			}
			else
			{
				mIndices.push_back((v * U_MAX) + u);
				mIndices.push_back(*mIndices.rbegin() + U_MAX);
			}
		}
	}

	// RayCast用
	for (size_t i = 0; i < mIndices.size() / 3; i += 3)
	{
		Face f;
		f.vertex[0] = mVertices[mIndices[i]].position;
		f.vertex[1] = mVertices[mIndices[i + 1]].position;
		f.vertex[2] = mVertices[mIndices[i + 2]].position;
		mFaces.push_back(f);
	}

	return CreateIndexBuffer(device);
}


// geometric_cylinder
HRESULT Cylinder::SetVertexBuffer(ID3D11Device* device)
{
	constexpr float LENGTH = 5.0f;
	constexpr float RADIUS = 1.75f;

	// 上側
	for (int u = 0; u < U_MAX; ++u)
	{
		float angle = DirectX::XMConvertToRadians(360.0f / U_MAX * u);
		float dx = sinf(angle) * RADIUS;
		float dy = LENGTH / 2.0f;
		float dz = cosf(angle) * RADIUS;

		Vertex temp = {};
		temp.position = Vector3(dx, dy, dz);
		temp.normal = temp.position;
		mVertices.push_back(temp);

		mAABB.min.x = Math::Min(mAABB.min.x, temp.position.x);
		mAABB.min.y = Math::Min(mAABB.min.y, temp.position.y);
		mAABB.min.z = Math::Min(mAABB.min.z, temp.position.z);
		mAABB.max.x = Math::Max(mAABB.max.x, temp.position.x);
		mAABB.max.y = Math::Max(mAABB.max.y, temp.position.y);
		mAABB.max.z = Math::Max(mAABB.max.z, temp.position.z);
	}

	// 下側
	for (int u = 0; u < U_MAX; ++u)
	{
		float angle = DirectX::XMConvertToRadians(360.0f / U_MAX * u);
		float dx = sinf(angle) * RADIUS;
		float dy = -LENGTH / 2.0f;
		float dz = cosf(angle) * RADIUS;
		

		Vertex temp = {};
		temp.position = Vector3(dx, dy, dz);
		temp.normal = temp.position;
		mVertices.push_back(temp);
	}

	mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	return CreateVertexBuffer(device);
}

HRESULT Cylinder::SetIndexBuffer(ID3D11Device* device)
{	
	// 上の円
	for (int u = 0; u < U_MAX - 2; ++u)
	{
		mIndices.push_back(0);
		mIndices.push_back(u + 2);
		mIndices.push_back(u + 1);
	}

	//// 円と円の間
	for (int u = 0; u < U_MAX; ++u)
	{
		if (u != U_MAX - 1)
		{			
			// 上2辺 ・ 下1辺
			mIndices.push_back(u + 1);
			mIndices.push_back(u + U_MAX);
			mIndices.push_back(u);

			// 上1辺 ・ 下2辺
			mIndices.push_back(u + U_MAX);
			mIndices.push_back(u + 1);
			mIndices.push_back(u + U_MAX + 1);

		}
		else
		{
			// 上2辺 ・ 下1辺
			mIndices.push_back(u - U_MAX + 1);
			mIndices.push_back(u + U_MAX);
			mIndices.push_back(u);

			// 上1辺 ・ 下2辺
			mIndices.push_back(u + U_MAX);
			mIndices.push_back(u - U_MAX + 1);
			mIndices.push_back(u + 1);

		}
	}

	// 下の円
	for (int u = U_MAX; u < U_MAX * 2 - 2; ++u)
	{
		mIndices.push_back(U_MAX);
		mIndices.push_back(u + 1);
		mIndices.push_back(u + 2);

	}

	// RayCast用
	for (size_t i = 0; i < mIndices.size() / 3; i += 3)
	{
		Face f;
		f.vertex[0] = mVertices[mIndices[i]].position;
		f.vertex[1] = mVertices[mIndices[i + 1]].position;
		f.vertex[2] = mVertices[mIndices[i + 2]].position;
		mFaces.push_back(f);
	}

	return CreateIndexBuffer(device);
}


// geometric_capsule
HRESULT Capsule::SetVertexBuffer(ID3D11Device* device)
{
	Vector3 sliceVector(1.0f, 1.0f, 1.0f);
	sliceVector.Normalize();

	//--  カプセル座標の単位ベクトルを作成する  --//
	Vector3 uz = mCenterTop - mCenterBottom; // uz : P2からP1へ向かう向きのベクトル
	uz.Normalize();
	Vector3 uy = Vector3::Cross(uz, sliceVector);	     // uy : vzとvと直角なベクトル
	Vector3 ux = Vector3::Cross(uz, uy);				 // vx : vzとvyと直角なベクトル

	// ベクトルの大きさをワールド座標でのradiusにする
	ux.Normalize();
	ux *= -mRadius;

	uy.Normalize();
	uy *= mRadius;

	uz *= mRadius;

	// 変数軍
	const float PI = 3.141592f;
	float invSlices = 1.0f / U_MAX;
	float invStacks = 1.0f / V_MAX;

	Vertex temp;
	{
		// 北半球

		// 最初の点
		temp.position = mCenterTop + uz;
		temp.normal = uz;
		mVertices.emplace_back(temp);

		for (UINT i = 0; i < V_MAX; i++)
		{
			FLOAT t = (PI * 0.5f) * invStacks * (i + 1);
			FLOAT r = sinf(t);
			Vector3 vx = ux * r;
			Vector3 vy = uy * r;
			Vector3 vz = uz * cosf(t);

			for (UINT j = 0; j < U_MAX; j++)
			{
				FLOAT u = (PI * 2.0f) * invSlices * j;
				Vector3 v = vx * cosf(u) + vy * sinf(u) + vz;
				temp.position = mCenterTop + v;
				temp.normal = v;
				mVertices.emplace_back(temp);
			}
		}

		// 南半球
		for (UINT i = 0; i < V_MAX; i++)
		{
			FLOAT t = (PI * 0.5f) * invStacks * i;
			FLOAT r = cosf(t);
			Vector3 vx = ux * r;
			Vector3 vy = uy * r;
			Vector3 vz = uz * -sinf(t);
			for (UINT j = 0; j < U_MAX; j++)
			{
				FLOAT u = (PI * 2.0f) * invSlices * j;
				Vector3 v = vx * cosf(u) + vy * sinf(u) + vz;
				temp.position = mCenterBottom + v;
				temp.normal = v;
				mVertices.emplace_back(temp);
			}
		}
		// 最後の点
		temp.position = mCenterBottom + -uz;
		temp.normal = -uz;
		mVertices.emplace_back(temp);
	}

	{
		//// 上の半球 と 下の半球を作って indexBuffer の方でくっつけたい;
		//// もしかしたら北半球と南半球の間に円作ってもいいかもしれない;(そんなことはなさそう) 

		////北半球
		//float radius = 10.0f;
		//for (int v = 0; v <= V_MAX; ++v)
		//{
		//	for (int u = 0; u < U_MAX; ++u) 
		//	{
		//		double theta = DirectX::XMConvertToRadians(90.0f * v / V_MAX);
		//		double phi = DirectX::XMConvertToRadians(360.0f * u / U_MAX);
		//		FLOAT X = static_cast<FLOAT>(sin(theta) * cos(phi) + radius);
		//		FLOAT Y = static_cast<FLOAT>(cos(theta));
		//		FLOAT Z = static_cast<FLOAT>(sin(theta) * sin(phi) + radius);
		//		Vertex temp = {};
		//		temp.position = Vector3(X, Y, Z) + mCenterTop;
		//		temp.normal = Vector3(X, Y, Z);
		//		mVertices.push_back(temp);
		//	}
		//}

		//// 南半球
		//for (int v = V_MAX; v >= 0; --v)
		//{
		//	for (int u = U_MAX; u > 0 ; --u)
		//	{
		//		double theta = DirectX::XMConvertToRadians(90.0f * v / V_MAX);
		//		double phi = DirectX::XMConvertToRadians(360.0f * u / U_MAX);
		//		FLOAT X = static_cast<FLOAT>(sin(theta) * cos(phi));
		//		FLOAT Y = static_cast<FLOAT>(-cos(theta));
		//		FLOAT Z = static_cast<FLOAT>(sin(theta) * sin(phi));
		//		Vertex temp = {};
		//		temp.position = Vector3(X, Y, Z) + mCenterBottom;
		//		temp.normal = Vector3(X, Y, Z);
		//		mVertices.push_back(temp);
		//	}
		//}

		//for (auto& v : mVertices)
		//{
		//	mAABB.min.x = Math::Min(mAABB.min.x, v.position.x);
		//	mAABB.min.y = Math::Min(mAABB.min.y, v.position.y);
		//	mAABB.min.z = Math::Min(mAABB.min.z, v.position.z);
		//	mAABB.max.x = Math::Max(mAABB.max.x, v.position.x);
		//	mAABB.max.y = Math::Max(mAABB.max.y, v.position.y);
		//	mAABB.max.z = Math::Max(mAABB.max.z, v.position.z);
		//}

		//mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		//return CreateVertexBuffer(device);

	}

	mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	return CreateVertexBuffer(device);
}

HRESULT Capsule::SetIndexBuffer(ID3D11Device* device)
{
	// 1番上の頂点の周り
	for (UINT i = 0; i < U_MAX; i++)
	{
		mIndices.emplace_back(0);
		mIndices.emplace_back(1 + i);
		mIndices.emplace_back(1 + (1 + i) % U_MAX);
	}

	// ↑から↓の間
	UINT imax = V_MAX * 2 - 1;
	for (UINT i = 0; i < imax; i++)
	{
		UINT ibase = 1 + U_MAX * i;
		for (UINT j = 0; j < U_MAX; j++)
		{
			UINT jnext = (j + 1) % U_MAX;
			mIndices.emplace_back(ibase + j);
			mIndices.emplace_back(ibase + j + U_MAX);
			mIndices.emplace_back(ibase + jnext);

			mIndices.emplace_back(ibase + jnext);
			mIndices.emplace_back(ibase + j + U_MAX);
			mIndices.emplace_back(ibase + jnext + U_MAX);
		}
	}

	// 1番下の頂点の周り
	UINT ibase = 1 + U_MAX * imax;
	UINT ilast = mVertices.size() - 1;
	for (UINT j = 0; j < U_MAX; j++)
	{
		mIndices.emplace_back(ibase + j);
		mIndices.emplace_back(ilast);
		mIndices.emplace_back(ibase + (j + 1) % U_MAX);
	}

	{
		//// †memo†
		//// (0) ~ (V_MAX * U_MAX + U_MAX - 1) で北半球おわり
		//// (V_MAX * U_MAX + U_MAX) ~ (V_MAX * U_MAX * 2 + U_MAX)(多分) で南半球終わり
		//// 上から下に頂点を打ってる
		//
		//// 北半球
		//for (int v = 0; v < V_MAX; ++v)
		//{
		//	for (int u = 0; u <= U_MAX; ++u)
		//	{
		//		if (u == U_MAX)
		//		{
		//			mIndices.push_back(v * U_MAX);
		//			mIndices.push_back((v + 1) * U_MAX);
		//		}
		//		else
		//		{
		//			mIndices.push_back((v * U_MAX) + u);
		//			mIndices.push_back(*mIndices.rbegin() + U_MAX);
		//		}
		//	}
		//}


		////北半球 と 南半球 の間 (円柱部分)
		//for (int u = 0; u < U_MAX; ++u)
		//{
		//	mIndices.push_back(V_MAX * U_MAX + U_MAX + u); // 南半球側
		//	mIndices.push_back(V_MAX * U_MAX + U_MAX - u - 1); // 北半球側
		//}
		//mIndices.push_back(V_MAX * U_MAX + U_MAX); // 円柱最後の点

		//// 南半球
		//for (int v = V_MAX; v < V_MAX * 2; ++v) // V は上から下まで増え続けるから初期値がV_MAX
		//{
		//	for (int u = 1; u <= U_MAX; ++u) // U は1週が U_MAX なだけで U_MAX 以上になることがないから 初期値0
		//	{
		//		mIndices.push_back(*mIndices.rbegin() + U_MAX); // 下の段
		//		mIndices.push_back(v * U_MAX + U_MAX + U_MAX - u); // 上の段
		//	}
		//}

		//// RayCast用
		//for (size_t i = 0; i < mIndices.size() / 3; i += 3)
		//{
		//	Face f;
		//	f.vertex[0] = mVertices[mIndices[i]].position;
		//	f.vertex[1] = mVertices[mIndices[i + 1]].position;
		//	f.vertex[2] = mVertices[mIndices[i + 2]].position;
		//	mFaces.push_back(f);
		//}

		//return CreateIndexBuffer(device);

	}

	return CreateIndexBuffer(device);
}