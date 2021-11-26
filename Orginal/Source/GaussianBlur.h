#pragma once
#include <memory>
#include <wrl.h>

#include "lib/ConstantBuffer.h"
#include "lib/RenderTarget.h"
#include "lib/Vector.h"

class Sprite;
class Shader;

/*
	�K�E�X�u���[��������N���X
*/
class GaussianBlur
{
	// �萔
	static const int SAMPLING_COUNT = 10; // X, Y���ꂼ��̃T���v�����O��

private: // �ϐ�

	// �萔�o�b�t�@�֌W
	struct CBuffer
	{
		float weight[SAMPLING_COUNT];
		Vector2 offset;
	};
	ConstantBuffer mConstBuffer;

	// �u���[�֌W
	std::unique_ptr<Shader> mHorizontalShader; // �������̃u���[�V�F�[�_
	RenderTarget mHorizontalBlur;			   // �������̃u���[�̃����_�[�e�N�X�`��
	std::unique_ptr<Shader> mVerticalShader;   // �c�����̃u���[�V�F�[�_
	RenderTarget mVerticalBlur;				   // �c�����̃u���[�̃����_�[�e�N�X�`��

	Vector2 mSize = {};			 // �����_�[�e�N�X�`���̃T�C�Y
	float mBlurStlength = 0.0f;	 // �u���[�̋���
	float mTexcelOffset = 1;	 // �e�N�Z���̃I�t�Z�b�g
	bool mWeightUpdated = false; // �E�F�C�g�̍X�V�ς݃t���O

private: // �֐�

	// �E�F�C�g���X�V����֐�
	void UpdateWeights();

public:

	// �R���X�g���N�^
	GaussianBlur();

	// �f�X�g���N�^
	~GaussianBlur();

	// �������֐�
	void Initialize(const Vector2& targetSize = Vector2::ZERO, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT, float blurStlength = 100.0f, float offset = 1);
	
	// �u���[���������e�N�X�`����`��(�v�FBlur()
	void Render();
	
	// �����̃e�N�X�`���Ƀu���[��������
	void Blur(const RenderTarget* orgSprite);

	// �u���[�̋�����ݒ�
	void SetBlurStlength(float stlength);

	RenderTarget* GetBlurTarget() { return &mVerticalBlur; };			// �u���[�ς݂̃����_�[�e�N�X�`�����擾
	void SetBlurTexture(UINT slot) { mVerticalBlur.SetTexture(slot); }	// �u���[�ς݂̃����_�[�e�N�X�`�����p�C�v���C���ɐݒ�
};