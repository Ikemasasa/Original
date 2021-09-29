#include "Matrix.h"

//---------------------------------------------------
// Matrix
//---------------------------------------------------
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
	Matrix b;

	float det = m[0] * m[5] * m[10] * m[15] + m[0] * m[6] * m[11] * m[13] + m[0] * m[7] * m[9] * m[14]
		+ m[1] * m[4] * m[11] * m[14] + m[1] * m[6] * m[8] * m[15] + m[1] * m[7] * m[10] * m[12]
		+ m[2] * m[4] * m[9] * m[15] + m[2] * m[5] * m[11] * m[12] + m[2] * m[7] * m[8] * m[13]
		+ m[3] * m[4] * m[10] * m[13] + m[3] * m[5] * m[8] * m[14] + m[3] * m[6] * m[9] * m[12]
		- m[0] * m[5] * m[11] * m[14] - m[0] * m[6] * m[9] * m[15] - m[0] * m[7] * m[10] * m[13]
		- m[1] * m[4] * m[10] * m[15] - m[1] * m[6] * m[11] * m[12] - m[1] * m[7] * m[8] * m[14]
		- m[2] * m[4] * m[11] * m[13] - m[2] * m[5] * m[8] * m[15] - m[2] * m[7] * m[9] * m[12]
		- m[3] * m[4] * m[9] * m[14] - m[3] * m[5] * m[10] * m[12] - m[3] * m[6] * m[8] * m[13];

	float inv_det = 1.0f / det;

	b.m[0] = inv_det * (m[5] * m[10] * m[15] + m[6] * m[11] * m[13] + m[7] * m[9] * m[14] - m[5] * m[11] * m[14] - m[6] * m[9] * m[15] - m[7] * m[10] * m[13]);
	b.m[1] = inv_det * (m[1] * m[11] * m[14] + m[2] * m[9] * m[15] + m[3] * m[10] * m[13] - m[1] * m[10] * m[15] - m[2] * m[11] * m[13] - m[3] * m[9] * m[14]);
	b.m[2] = inv_det * (m[1] * m[6] * m[15] + m[2] * m[7] * m[13] + m[3] * m[5] * m[14] - m[1] * m[7] * m[14] - m[2] * m[5] * m[15] - m[3] * m[6] * m[13]);
	b.m[3] = inv_det * (m[1] * m[7] * m[10] + m[2] * m[5] * m[11] + m[3] * m[6] * m[9] - m[1] * m[6] * m[11] - m[2] * m[7] * m[9] - m[3] * m[5] * m[10]);

	b.m[4] = inv_det * (m[4] * m[11] * m[14] + m[6] * m[8] * m[15] + m[7] * m[10] * m[12] - m[4] * m[10] * m[15] - m[6] * m[11] * m[12] - m[7] * m[8] * m[14]);
	b.m[5] = inv_det * (m[0] * m[10] * m[15] + m[2] * m[11] * m[12] + m[3] * m[8] * m[14] - m[0] * m[11] * m[14] - m[2] * m[8] * m[15] - m[3] * m[10] * m[12]);
	b.m[6] = inv_det * (m[0] * m[7] * m[14] + m[2] * m[4] * m[15] + m[3] * m[6] * m[12] - m[0] * m[6] * m[15] - m[2] * m[7] * m[12] - m[3] * m[4] * m[14]);
	b.m[7] = inv_det * (m[0] * m[6] * m[11] + m[2] * m[7] * m[8] + m[3] * m[4] * m[10] - m[0] * m[7] * m[10] - m[2] * m[4] * m[11] - m[3] * m[6] * m[8]);

	b.m[8] = inv_det * (m[4] * m[9] * m[15] + m[5] * m[11] * m[12] + m[7] * m[8] * m[13] - m[4] * m[11] * m[13] - m[5] * m[8] * m[15] - m[7] * m[9] * m[12]);
	b.m[9] = inv_det * (m[0] * m[11] * m[13] + m[1] * m[8] * m[15] + m[3] * m[9] * m[12] - m[0] * m[9] * m[15] - m[1] * m[11] * m[12] - m[3] * m[8] * m[13]);
	b.m[10] = inv_det * (m[0] * m[5] * m[15] + m[1] * m[7] * m[12] + m[3] * m[4] * m[13] - m[0] * m[7] * m[13] - m[1] * m[4] * m[15] - m[3] * m[5] * m[12]);
	b.m[11] = inv_det * (m[0] * m[7] * m[9] + m[1] * m[4] * m[11] + m[3] * m[5] * m[8] - m[0] * m[5] * m[11] - m[1] * m[7] * m[8] - m[3] * m[4] * m[9]);

	b.m[12] = inv_det * (m[4] * m[10] * m[13] + m[5] * m[8] * m[14] + m[6] * m[9] * m[12] - m[4] * m[9] * m[14] - m[5] * m[10] * m[12] - m[6] * m[8] * m[13]);
	b.m[13] = inv_det * (m[0] * m[9] * m[14] + m[1] * m[10] * m[12] + m[2] * m[8] * m[13] - m[0] * m[10] * m[13] - m[1] * m[8] * m[14] - m[2] * m[9] * m[12]);
	b.m[14] = inv_det * (m[0] * m[6] * m[13] + m[1] * m[4] * m[14] + m[2] * m[5] * m[12] - m[0] * m[5] * m[14] - m[1] * m[6] * m[12] - m[2] * m[4] * m[13]);
	b.m[15] = inv_det * (m[0] * m[5] * m[10] + m[1] * m[6] * m[8] + m[2] * m[4] * m[9] - m[0] * m[6] * m[9] - m[1] * m[4] * m[10] - m[2] * m[5] * m[8]);


	_11 = b._11;	_12 = b._12;	_13 = b._13;	_14 = b._14;
	_21 = b._21;	_22 = b._22;	_23 = b._23;	_24 = b._24;
	_31 = b._31;	_32 = b._32;	_33 = b._33;	_34 = b._34;
	_41 = b._41;	_42 = b._42;	_43 = b._43;	_44 = b._44;
}

void Matrix::Interporate(Matrix& target, float rate)
{
	Matrix A = *this * (1.0f - rate) + target * rate;
	*this = A;
}

void Matrix::Interporate(Matrix* result, const Matrix& mat1, const Matrix& mat2, float rate)
{
	*result = mat1 * (1.0f - rate) + mat2 * rate;
}

void Matrix::LookAtLH(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	Vector3 xAxis, yAxis, zAxis;
	zAxis = target - pos;
	zAxis.Normalize();
	xAxis = Vector3::Cross(up, zAxis);
	xAxis.Normalize();
	yAxis = Vector3::Cross(zAxis, xAxis);
	yAxis.Normalize();

	_11 = xAxis.x;	_12 = yAxis.x;	_13 = zAxis.x;	_14 = 0;
	_21 = xAxis.y;	_22 = yAxis.y;	_23 = zAxis.y;	_24 = 0;
	_31 = xAxis.z;	_32 = yAxis.z;	_33 = zAxis.z;	_34 = 0;
	_41 = -Vector3::Dot(xAxis, pos);
	_42 = -Vector3::Dot(yAxis, pos);
	_43 = -Vector3::Dot(zAxis, pos);
	_44 = 1;
}

void Matrix::PerspectiveFov(float fovY, float aspect, float znear, float zfar)
{
	float top = znear * tanf(fovY * 0.5f);
	float bottom = -top;
	float left = bottom * aspect;
	float right = top * aspect;

	_11 = 2 * znear / (right - left);
	_12 = 0;
	_13 = 0;
	_14 = 0;

	_21 = 0;
	_22 = 2 * znear / (top - bottom);
	_23 = 0;
	_24 = 0;

	_31 = (right + left) / (right - left);
	_32 = (top + bottom) / (top - bottom);
	_33 = (zfar + znear) / (zfar - znear);
	_34 = 1;

	_41 = 0;
	_42 = 0;
	_43 = -(zfar * znear / (zfar - znear));
	_44 = 0;
}

void Matrix::Ortho(float w, float h, float zn, float zf)
{
	_11 = 2 / w; _12 = 0;     _13 = 0;				_14 = 0;
	_21 = 0;     _22 = 2 / h; _23 = 0;				_24 = 0;
	_31 = 0;     _32 = 0;     _33 = 1 / (zn - zf);  _34 = 0;
	_41 = 0;     _42 = 0;     _43 = zn / (zn - zf); _44 = 1;
}

void Matrix::SRT(const Vector3& s, const Vector3& r, const Vector3& t)
{
	// âÒì]
	float sinX = sinf(r.x), cosX = cosf(r.x);
	float sinY = sinf(r.y), cosY = cosf(r.y);
	float sinZ = sinf(r.z), cosZ = cosf(r.z);

	_11 = cosZ * cosY + sinZ * sinX * sinY;
	_12 = sinZ * cosX;
	_13 = -cosZ * sinY + sinZ * sinX * cosY;
	_14 = 0;

	_21 = -sinZ * cosY + cosZ * sinX * sinY;
	_22 = cosZ * cosX;
	_23 = sinZ * sinY + cosZ * sinX * cosY;
	_24 = 0;

	_31 = cosX * sinY;
	_32 = -sinX;
	_33 = cosX * cosY;
	_34 = 0;

	_41 = 0;
	_42 = 0;
	_43 = 0;
	_44 = 1;

	// ÉXÉPÅ[Éã
	_11 *= s.x;
	_12 *= s.x;
	_13 *= s.x;
	_21 *= s.y;
	_22 *= s.y;
	_23 *= s.y;
	_31 *= s.z;
	_32 *= s.z;
	_33 *= s.z;

	// à⁄ìÆ
	_41 = t.x;
	_42 = t.y;
	_43 = t.z;
}

Matrix Matrix::operator+(const Matrix& mat) const
{
	Matrix m;
	for (int i = 0; i < 16; ++i)
	{
		m.m[i] = this->m[i] + mat.m[i];
	}
	return m;
}

Matrix Matrix::operator*(const Matrix& mat) const
{
	Matrix m;
	m.Multiply(*this, mat);
	return m;
}

Matrix Matrix::operator*(float val) const
{
	Matrix m;
	for (int i = 0; i < 16; ++i)
	{
		m.m[i] = this->m[i] * val;
	}
	return m;
}

Matrix& Matrix::operator*=(const Matrix& mat)
{
	Multiply(*this, mat);
	return *this;
}