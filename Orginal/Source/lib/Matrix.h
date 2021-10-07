#pragma once
#include "Vector.h"

class Matrix
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[16];
	};

private:
	void Multiply(const Matrix& mat1, const Matrix& mat2);
	void Multiply(float val);

public:
	Matrix() = default;
	~Matrix() = default;

	float* Data() { return m; }
	void Identity();
	void Inverse();
	void Interporate(Matrix& target, float rate);
	static void Interporate(Matrix* result, const Matrix& mat1, const Matrix& mat2, float rate);

	// カメラ系
	void LookAtLH(const Vector3& pos, const Vector3& target, const Vector3& up = Vector3(0, 1, 0)); // ビュー行列作成
	void PerspectiveFov(float fovY, float aspect, float znear, float zfar);						  // 透視投影変換行列作成
	void Ortho(float w, float h, float zn, float zf);											  // 平行投影変換行列作成

	// 座標変換系
	void SRT(const Vector3& s, const Vector3& r, const Vector3& t);

	// operator
	Matrix operator +(const Matrix& mat) const;
	Matrix operator *(const Matrix& mat) const;
	Matrix operator *(float val) const;
	Matrix& operator *=(const Matrix& mat);
};