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
		// �ʒ��_�擾
		DirectX::XMVECTOR vertex[3] = {};
		vertex[0] = DirectX::XMLoadFloat3(&it.vertex[0]); // a
		vertex[1] = DirectX::XMLoadFloat3(&it.vertex[1]); // b 
		vertex[2] = DirectX::XMLoadFloat3(&it.vertex[2]); // c

		// 3�ӎZ�o
		DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(vertex[1], vertex[0]);
		DirectX::XMVECTOR bc = DirectX::XMVectorSubtract(vertex[2], vertex[1]);
		DirectX::XMVECTOR ca = DirectX::XMVectorSubtract(vertex[0], vertex[2]);

		// �O�ςɂ��@���Z�o
		DirectX::XMVECTOR nor;
		nor = DirectX::XMVector3Cross(ab, bc);
		nor = DirectX::XMVector3Normalize(nor);

		// ���ς̌��ʂ��v���X�Ȃ�Η�����
		DirectX::XMVECTOR dot = DirectX::XMVector3Dot(nor, dir);
		if (DirectX::XMVectorGetX(dot) > 0) continue;

		// ��_�Z�o
		DirectX::XMVECTOR v0 = DirectX::XMVectorSubtract(vertex[0], start); // �|���S����1���_���烌�C�̊J�n�n�_�̃x�N�g��
		DirectX::XMVECTOR d1 = DirectX::XMVector3Dot(v0, nor);				// ���̃x�N�g���ƃ|���S���̖@���̓���(�ˉe���o��)
		DirectX::XMVECTOR t = DirectX::XMVectorDivide(d1, dot);				//  ���̃x�N�g���Ɩ@���̊���������
		float ft = 0.0f;
		DirectX::XMStoreFloat(&ft, t);
		if (ft < 0.0f || ft > neart) continue;

		// ��_
		DirectX::XMVECTOR cp = DirectX::XMVectorAdd(start, DirectX::XMVectorMultiply(dir, t));

		// ���_����
		// 1��
		DirectX::XMVECTOR tempV = DirectX::XMVectorSubtract(vertex[0], cp); //pa
		DirectX::XMVECTOR tempN = DirectX::XMVector3Cross(tempV, ab);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // ���ς̌��ʃ}�C�i�X�Ȃ�O��

		// 2��
		tempV = DirectX::XMVectorSubtract(vertex[1], cp); //pb
		tempN = DirectX::XMVector3Cross(tempV, bc);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // ���ς̌��ʃ}�C�i�X�Ȃ�O��

		// 3��
		tempV = DirectX::XMVectorSubtract(vertex[2], cp); //pc
		tempN = DirectX::XMVector3Cross(tempV, ca);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // ���ς̌��ʃ}�C�i�X�Ȃ�O��

		// ���ۑ�
		position = cp;
		normal = nor;
		neart = ft;
		ret = it.materialIndex;
	}
	if (ret != -1)
	{
		// ��������
		DirectX::XMStoreFloat3(outPos, position);
		DirectX::XMStoreFloat3(outNormal, normal);
	}

	// �ł��߂��q�b�g�ʒu�܂ł̋���
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

	// RayCast�p
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

	// RayCast�p
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

	// �㑤
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

	// ����
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
	// ��̉~
	for (int u = 0; u < U_MAX - 2; ++u)
	{
		mIndices.push_back(0);
		mIndices.push_back(u + 2);
		mIndices.push_back(u + 1);
	}

	//// �~�Ɖ~�̊�
	for (int u = 0; u < U_MAX; ++u)
	{
		if (u != U_MAX - 1)
		{			
			// ��2�� �E ��1��
			mIndices.push_back(u + 1);
			mIndices.push_back(u + U_MAX);
			mIndices.push_back(u);

			// ��1�� �E ��2��
			mIndices.push_back(u + U_MAX);
			mIndices.push_back(u + 1);
			mIndices.push_back(u + U_MAX + 1);

		}
		else
		{
			// ��2�� �E ��1��
			mIndices.push_back(u - U_MAX + 1);
			mIndices.push_back(u + U_MAX);
			mIndices.push_back(u);

			// ��1�� �E ��2��
			mIndices.push_back(u + U_MAX);
			mIndices.push_back(u - U_MAX + 1);
			mIndices.push_back(u + 1);

		}
	}

	// ���̉~
	for (int u = U_MAX; u < U_MAX * 2 - 2; ++u)
	{
		mIndices.push_back(U_MAX);
		mIndices.push_back(u + 1);
		mIndices.push_back(u + 2);

	}

	// RayCast�p
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

	//--  �J�v�Z�����W�̒P�ʃx�N�g�����쐬����  --//
	Vector3 uz = mCenterTop - mCenterBottom; // uz : P2����P1�֌����������̃x�N�g��
	uz.Normalize();
	Vector3 uy = Vector3::Cross(uz, sliceVector);	     // uy : vz��v�ƒ��p�ȃx�N�g��
	Vector3 ux = Vector3::Cross(uz, uy);				 // vx : vz��vy�ƒ��p�ȃx�N�g��

	// �x�N�g���̑傫�������[���h���W�ł�radius�ɂ���
	ux.Normalize();
	ux *= -mRadius;

	uy.Normalize();
	uy *= mRadius;

	uz *= mRadius;

	// �ϐ��R
	const float PI = 3.141592f;
	float invSlices = 1.0f / U_MAX;
	float invStacks = 1.0f / V_MAX;

	Vertex temp;
	{
		// �k����

		// �ŏ��̓_
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

		// �씼��
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
		// �Ō�̓_
		temp.position = mCenterBottom + -uz;
		temp.normal = -uz;
		mVertices.emplace_back(temp);
	}

	{
		//// ��̔��� �� ���̔���������� indexBuffer �̕��ł���������;
		//// ������������k�����Ɠ씼���̊Ԃɉ~����Ă�������������Ȃ�;(����Ȃ��Ƃ͂Ȃ�����) 

		////�k����
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

		//// �씼��
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
	// 1�ԏ�̒��_�̎���
	for (UINT i = 0; i < U_MAX; i++)
	{
		mIndices.emplace_back(0);
		mIndices.emplace_back(1 + i);
		mIndices.emplace_back(1 + (1 + i) % U_MAX);
	}

	// �����火�̊�
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

	// 1�ԉ��̒��_�̎���
	UINT ibase = 1 + U_MAX * imax;
	UINT ilast = mVertices.size() - 1;
	for (UINT j = 0; j < U_MAX; j++)
	{
		mIndices.emplace_back(ibase + j);
		mIndices.emplace_back(ilast);
		mIndices.emplace_back(ibase + (j + 1) % U_MAX);
	}

	{
		//// ��memo��
		//// (0) ~ (V_MAX * U_MAX + U_MAX - 1) �Ŗk���������
		//// (V_MAX * U_MAX + U_MAX) ~ (V_MAX * U_MAX * 2 + U_MAX)(����) �œ씼���I���
		//// �ォ�牺�ɒ��_��ł��Ă�
		//
		//// �k����
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


		////�k���� �� �씼�� �̊� (�~������)
		//for (int u = 0; u < U_MAX; ++u)
		//{
		//	mIndices.push_back(V_MAX * U_MAX + U_MAX + u); // �씼����
		//	mIndices.push_back(V_MAX * U_MAX + U_MAX - u - 1); // �k������
		//}
		//mIndices.push_back(V_MAX * U_MAX + U_MAX); // �~���Ō�̓_

		//// �씼��
		//for (int v = V_MAX; v < V_MAX * 2; ++v) // V �͏ォ�牺�܂ő��������邩�珉���l��V_MAX
		//{
		//	for (int u = 1; u <= U_MAX; ++u) // U ��1�T�� U_MAX �Ȃ����� U_MAX �ȏ�ɂȂ邱�Ƃ��Ȃ����� �����l0
		//	{
		//		mIndices.push_back(*mIndices.rbegin() + U_MAX); // ���̒i
		//		mIndices.push_back(v * U_MAX + U_MAX + U_MAX - u); // ��̒i
		//	}
		//}

		//// RayCast�p
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