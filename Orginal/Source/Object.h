#pragma once

#include <memory>

#include "lib/Vector.h"
#include "lib/Matrix.h"

class SkinnedMesh;
class Shader;

// �����Ȃ����b�V���������N���X(���[�V�����̍X�V�Ƃ������Ă��Ȃ�)
class Object
{
private: // �ϐ�

	std::shared_ptr<SkinnedMesh> mMesh = nullptr;	 // �I���W�i���̃��b�V��(�`��p)
	std::shared_ptr<SkinnedMesh> mMeshLow = nullptr; // ���E�|���̃��b�V��
	std::shared_ptr<SkinnedMesh> mMeshCol = nullptr; // �R���W�����p�̃��b�V��
	Matrix mWorld = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }; // ���[���h�s��
	int mID = -1; // ID

protected:
	Vector3 mPos = { 0,0,0 };     // ���W
	Vector3 mScale = { 1, 1, 1 }; // �X�P�[��
	Vector3 mAngle = { 0,0,0 };	  // �p�x

public: // �֐�

	// �R���X�g���N�^
	Object(int id);
	Object(const Object* obj);

	// �f�X�g���N�^
	~Object() = default;

	// ���[���h�s����X�V����֐�
	void UpdateWorld();

	// �I���W�i�����b�V����`��
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// ���E�|�����b�V����`��
	void RenderLow(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// ���E�|���̃��b�V����Ray���΂�(Org���Ă����͈̂��S���邽�߂̃E�\)
	int RayPickOrg(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);

	// �R���W�����p�̃��b�V����Ray���΂�
	int RayPickCol(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);

	//�Q�b�^�[
	Vector3 GetPos() const { return mPos; }
	Vector3 GetScale() const { return mScale; }
	Vector3 GetAngle() const { return mAngle; }
	int GetID() const { return mID; }

	// �Z�b�^�[
	void SetPos(const Vector3& p) { mPos = p; }
	void SetScale(const Vector3& s) { mScale = s; }
	void SetAngle(const Vector3& a) { mAngle = a; }
	void SetAngleY(float y) { mAngle.y = y; }
	void SetID(const int id) { mID = id; }

};