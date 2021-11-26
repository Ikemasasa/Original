#pragma once
#include <DirectXmath.h>
#include <memory>

#include "ConstantBuffer.h"
#include "Matrix.h"
#include "Vector.h"

class Sprite;
class Shader;

// �X�J�C�{�b�N�X��`�悷��N���X
class Skybox
{
	// �ϐ�
	struct CbSky
	{
		Vector4 cameraPos;
		Matrix invView;
		Matrix invProj;
	};
	ConstantBuffer mConstBuffer;	// �萔�o�b�t�@

	std::unique_ptr<Sprite> mTexture; // �X�J�C�{�b�N�X�̉摜
	std::unique_ptr<Shader> mShader;  // �X�J�C�{�b�N�X�V�F�[�_
	Vector3 mEyePos = {};			  // �J�������W
	UINT mTextureSlot = -1;			  // �p�C�v���C���ɃZ�b�g����X���b�g

public:// �֐�

	// �R���X�g���N�^
	Skybox() = default;

	// �f�X�g���N�^
	~Skybox() = default;
	
	// ������
	void Initialize(const wchar_t* filename);
	
	// ���
	void Release();

	// �`��
	void Render(const Matrix& view, const Matrix& proj);

	// �J�������W�ݒ�
	void SetEyePos(Vector3 eye);

	// �摜���p�C�v���C���ɐݒ�
	void SetEnvTexture(UINT slot);
};