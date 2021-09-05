#include "Framework.h"
#include "GeometricPrimitive.h"
#include "ResourceManager.h"
#include <vector>
#include "Math.h"

GeometricPrimitive::GeometricPrimitive()
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
	mConstBuffer = nullptr;
	mTopology = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	mVertices.clear();
	mIndices.clear();
}

GeometricPrimitive::~GeometricPrimitive()
{
	mShader.UnLoad();
	mVertices.clear();
	mIndices.clear();
}

void GeometricPrimitive::Initialize()
{
	auto device = FRAMEWORK.GetDevice();

	HRESULT hr;
	
	mShader.Load(L"Shaders/GeometricPrimitive.fx", "VSMain", "PSMain");


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

HRESULT GeometricPrimitive::CreateVertexBuffer(ID3D11Device* device)
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

HRESULT GeometricPrimitive::CreateIndexBuffer(ID3D11Device* device)
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


void GeometricPrimitive::Render(const DirectX::XMFLOAT4X4& wvp, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& lightDirection, const DirectX::XMFLOAT4& color)
{
	auto context = FRAMEWORK.GetContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	mShader.Activate();

	context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(mTopology);
	
	Cbuffer cb = {};
	cb.wvp = wvp;
	cb.world = world;
	cb.lightDir = lightDirection;
	cb.materialColor = color;
	context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, &cb, 0, 0);
	context->VSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());

	context->DrawIndexed(mIndices.size(), 0, 0);

	context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

int GeometricPrimitive::RayCast(const DirectX::XMFLOAT3& sp, const DirectX::XMFLOAT3& ep, DirectX::XMFLOAT3* outPos, DirectX::XMFLOAT3* outNormal, float* outLen)
{
	int ret = -1;
	DirectX::XMVECTOR start = DirectX::XMLoadFloat3(&sp);
	DirectX::XMVECTOR end = DirectX::XMLoadFloat3(&ep);
	DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);
	DirectX::XMVECTOR length = DirectX::XMVector3Length(vec);
	DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(vec);
	float neart;
	DirectX::XMStoreFloat(&neart, length);


	DirectX::XMVECTOR position = {}, normal = {};
	for (const auto& it : mFaces)
	{
		// 面頂点取得
		DirectX::XMVECTOR vertex[3] = {};
		vertex[0] = DirectX::XMLoadFloat3(&it.vertex[0]); // a
		vertex[1] = DirectX::XMLoadFloat3(&it.vertex[1]); // b 
		vertex[2] = DirectX::XMLoadFloat3(&it.vertex[2]); // c

		// 3辺算出
		DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(vertex[1], vertex[0]);
		DirectX::XMVECTOR bc = DirectX::XMVectorSubtract(vertex[2], vertex[1]);
		DirectX::XMVECTOR ca = DirectX::XMVectorSubtract(vertex[0], vertex[2]);

		// 外積による法線算出
		DirectX::XMVECTOR nor;
		nor = DirectX::XMVector3Cross(ab, bc);
		nor = DirectX::XMVector3Normalize(nor);

		// 内積の結果がプラスならば裏向き
		DirectX::XMVECTOR dot = DirectX::XMVector3Dot(nor, dir);
		if (DirectX::XMVectorGetX(dot) > 0) continue;

		// 交点算出
		DirectX::XMVECTOR v0 = DirectX::XMVectorSubtract(vertex[0], start); // ポリゴンの1頂点からレイの開始地点のベクトル
		DirectX::XMVECTOR d1 = DirectX::XMVector3Dot(v0, nor);				// ↑のベクトルとポリゴンの法線の内積(射影を出す)
		DirectX::XMVECTOR t = DirectX::XMVectorDivide(d1, dot);				//  ↑のベクトルと法線の割合をだす
		float ft = 0.0f;
		DirectX::XMStoreFloat(&ft, t);
		if (ft < 0.0f || ft > neart) continue;

		// 交点
		DirectX::XMVECTOR cp = DirectX::XMVectorAdd(start, DirectX::XMVectorMultiply(dir, t));

		// 内点判定
		// 1つ目
		DirectX::XMVECTOR tempV = DirectX::XMVectorSubtract(vertex[0], cp); //pa
		DirectX::XMVECTOR tempN = DirectX::XMVector3Cross(tempV, ab);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // 内積の結果マイナスなら外側

		// 2つ目
		tempV = DirectX::XMVectorSubtract(vertex[1], cp); //pb
		tempN = DirectX::XMVector3Cross(tempV, bc);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // 内積の結果マイナスなら外側

		// 3つ目
		tempV = DirectX::XMVectorSubtract(vertex[2], cp); //pc
		tempN = DirectX::XMVector3Cross(tempV, ca);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // 内積の結果マイナスなら外側

		// 情報保存
		position = cp;
		normal = nor;
		neart = ft;
		ret = it.materialIndex;
	}
	if (ret != -1)
	{
		// 当たった
		DirectX::XMStoreFloat3(outPos, position);
		DirectX::XMStoreFloat3(outNormal, normal);
	}

	// 最も近いヒット位置までの距離
	*outLen = neart;
	return ret;


}

/* memo 
	u = x,
	v = y,
*/

// geometric_cube
HRESULT GeometricCube::SetVertexBuffer(ID3D11Device* device)
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

HRESULT GeometricCube::SetIndexBuffer(ID3D11Device* device)
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
HRESULT GeometricSphere::SetVertexBuffer(ID3D11Device* device)
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

HRESULT GeometricSphere::SetIndexBuffer(ID3D11Device* device)
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
HRESULT GeometricCylinder::SetVertexBuffer(ID3D11Device* device)
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

HRESULT GeometricCylinder::SetIndexBuffer(ID3D11Device* device)
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
HRESULT GeometricCapsule::SetVertexBuffer(ID3D11Device* device)
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

HRESULT GeometricCapsule::SetIndexBuffer(ID3D11Device* device)
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