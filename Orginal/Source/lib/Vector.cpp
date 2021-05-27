#include "Vector.h"
#include "Framework.h"

#include "Math.h"

Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	t = Math::Clamp01(t);

	Vector3 ret;
	ret.x = v1.x + t * (v2.x - v1.x);
	ret.y = v1.y + t * (v2.y - v1.y);
	ret.z = v1.z + t * (v2.z - v1.z);
	return ret;
}

Vector2 Vector3::WorldToScreen(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj) const
{
	DirectX::XMVECTOR pos = DirectX::XMVectorSet( x, y, z, 1.0f );

	DirectX::XMMATRIX matView = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX matProj = DirectX::XMLoadFloat4x4(&proj);

	ID3D11DeviceContext* context = FRAMEWORK.GetContext();
	D3D11_VIEWPORT vp;
	UINT numViewport = 1;
	context->RSGetViewports(&numViewport, &vp);

	DirectX::XMVECTOR ret;
	ret = DirectX::XMVector3Project(pos, vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height, vp.MinDepth, vp.MaxDepth, 
									matProj, matView, DirectX::XMMatrixIdentity());

	return Vector2(DirectX::XMVectorGetX(ret), DirectX::XMVectorGetY(ret));

	//// ビュー変換
	//pos = DirectX::XMVector3TransformCoord(pos, matView);

	//// プロジェクション変換
	//pos = DirectX::XMVector3TransformCoord(pos, matProj);

	//// w で割る(同時座標を考慮
	//pos = DirectX::XMVectorDivide(pos, DirectX::XMVectorSet(pos.m128_f32[3], pos.m128_f32[3], pos.m128_f32[3], pos.m128_f32[3])); 

	//ID3D11DeviceContext* context = FRAMEWORK.GetContext();
	//D3D11_VIEWPORT vp;
	//UINT numViewport = 1;
	//context->RSGetViewports(&numViewport, &vp);

	//// スクリーン変換
	//Vector2 ret;
	//ret.x = (pos.m128_f32[0] + 1.0f) / 2.0f * vp.Width;
	//ret.y = (pos.m128_f32[1] - 1.0f) / 2.0f * -1.0f * vp.Height;

	//return ret;
}

void Matrix::Identity()
{
	_11 = 1; _12 = 0; _13 = 0; _14 = 0;
	_21 = 0; _22 = 1; _23 = 0; _24 = 0;
	_31 = 0; _32 = 0; _33 = 1; _34 = 0;
	_41 = 0; _42 = 0; _43 = 0; _44 = 1;
}

void Matrix::Multiply(const Matrix& mat1, const Matrix& mat2)
{
	_11 = mat1._11 * mat2._11 + mat1._12 * mat2._21 + mat1._13 * mat2._31 + mat1._14 * mat2._41;
	_12 = mat1._11 * mat2._12 + mat1._12 * mat2._22 + mat1._13 * mat2._32 + mat1._14 * mat2._42;
	_13 = mat1._11 * mat2._13 + mat1._12 * mat2._23 + mat1._13 * mat2._33 + mat1._14 * mat2._43;
	_14 = mat1._11 * mat2._14 + mat1._12 * mat2._24 + mat1._13 * mat2._34 + mat1._14 * mat2._44;

	_21 = mat1._21 * mat2._11 + mat1._22 * mat2._21 + mat1._23 * mat2._31 + mat1._24 * mat2._41;
	_22 = mat1._21 * mat2._12 + mat1._22 * mat2._22 + mat1._23 * mat2._32 + mat1._24 * mat2._42;
	_23 = mat1._21 * mat2._13 + mat1._22 * mat2._23 + mat1._23 * mat2._33 + mat1._24 * mat2._43;
	_24 = mat1._21 * mat2._14 + mat1._22 * mat2._24 + mat1._23 * mat2._34 + mat1._24 * mat2._44;

	_31 = mat1._31 * mat2._11 + mat1._32 * mat2._21 + mat1._33 * mat2._31 + mat1._34 * mat2._41;
	_32 = mat1._31 * mat2._12 + mat1._32 * mat2._22 + mat1._33 * mat2._32 + mat1._34 * mat2._42;
	_33 = mat1._31 * mat2._13 + mat1._32 * mat2._23 + mat1._33 * mat2._33 + mat1._34 * mat2._43;
	_34 = mat1._31 * mat2._14 + mat1._32 * mat2._24 + mat1._33 * mat2._34 + mat1._34 * mat2._44;

	_41 = mat1._41 * mat2._11 + mat1._42 * mat2._21 + mat1._43 * mat2._31 + mat1._44 * mat2._41;
	_42 = mat1._41 * mat2._12 + mat1._42 * mat2._22 + mat1._43 * mat2._32 + mat1._44 * mat2._42;
	_43 = mat1._41 * mat2._13 + mat1._42 * mat2._23 + mat1._43 * mat2._33 + mat1._44 * mat2._43;
	_44 = mat1._41 * mat2._14 + mat1._42 * mat2._24 + mat1._43 * mat2._34 + mat1._44 * mat2._44;
}

void Matrix::Multiply(float val)
{
	_11 *= val; _12 *= val; _13 *= val; _14 *= val;
	_21 *= val; _22 *= val; _23 *= val; _24 *= val;
	_31 *= val; _32 *= val; _33 *= val; _34 *= val;
	_41 *= val; _42 *= val; _43 *= val; _44 *= val;
}

void Matrix::Inverse()
{
	Matrix m;
	m._11 = _22 * (_33 * _44 - _34 * _43) + _23 * (_34 * _42 - _32 * _44) + _24 * (_32 * _43 - _33 * _42);
	m._12 = _32 * (_43 * _14 - _44 * _13) + _33 * (_44 * _12 - _42 * _14) + _34 * (_42 * _13 - _43 * _12);
	m._13 = _42 * (_13 * _24 - _14 * _23) + _43 * (_14 * _22 - _12 * _24) + _44 * (_12 * _23 - _13 * _22);
	m._14 = _12 * (_23 * _34 - _24 * _33) + _13 * (_24 * _32 - _22 * _34) + _14 * (_22 * _33 - _23 * _32);

	m._21 = _23 * (_34 * _41 - _31 * _44) + _24 * (_31 * _43 - _33 * _41) + _21 * (_33 * _44 - _34 * _43);
	m._22 = _33 * (_44 * _11 - _41 * _14) + _34 * (_41 * _13 - _43 * _11) + _31 * (_43 * _14 - _44 * _13);
	m._23 = _43 * (_14 * _21 - _11 * _24) + _44 * (_11 * _23 - _13 * _21) + _41 * (_13 * _24 - _14 * _23);
	m._24 = _13 * (_24 * _31 - _21 * _34) + _14 * (_21 * _33 - _23 * _31) + _11 * (_23 * _34 - _24 * _33);

	m._31 = _24 * (_31 * _42 - _32 * _41) + _21 * (_32 * _44 - _34 * _42) + _22 * (_34 * _41 - _31 * _44);
	m._32 = _34 * (_41 * _12 - _42 * _11) + _31 * (_42 * _14 - _44 * _12) + _32 * (_44 * _11 - _41 * _14);
	m._33 = _44 * (_11 * _22 - _12 * _21) + _41 * (_12 * _24 - _14 * _22) + _42 * (_14 * _21 - _11 * _24);
	m._34 = _14 * (_21 * _32 - _22 * _31) + _11 * (_22 * _34 - _24 * _32) + _12 * (_24 * _31 - _21 * _34);

	m._41 = _21 * (_32 * _43 - _33 * _42) + _22 * (_33 * _41 - _31 * _43) + _23 * (_31 * _42 - _32 * _41);
	m._42 = _31 * (_42 * _13 - _43 * _12) + _32 * (_43 * _11 - _41 * _13) + _33 * (_41 * _12 - _42 * _11);
	m._43 = _41 * (_12 * _23 - _13 * _22) + _42 * (_13 * _21 - _11 * _23) + _43 * (_11 * _22 - _12 * _21);
	m._44 = _11 * (_22 * _33 - _23 * _32) + _12 * (_23 * _31 - _21 * _33) + _13 * (_21 * _32 - _22 * _31);

	float det = 1.0f / (_11 * m._11 + _21 * m._12 + _31 * m._13 + _41 * m._14);

	_11 = m._11 * det;	_12 = m._12 * det;	_13 = m._13 * det;	_14 = m._14 * det;
	_21 = m._21 * det;	_22 = m._22 * det;	_23 = m._23 * det;	_24 = m._24 * det;
	_31 = m._31 * det;	_32 = m._32 * det;	_33 = m._33 * det;	_34 = m._34 * det;
	_41 = m._41 * det;	_42 = m._42 * det;	_43 = m._43 * det;	_44 = m._44 * det;
}

