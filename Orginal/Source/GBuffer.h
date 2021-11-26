#pragma once
#include <memory>

#include "lib/RenderTarget.h"

class Shader;

/*
	GBuffer�N���X
	�f�B�t�@�[�h�����_�����O�p
	���̓J���[�A���W�A�@�� ���ۑ��ł���
*/
class GBuffer
{
public: // �ϐ�
	enum Kind { COLOR, POS, NORMAL, NUM };

private:
	std::unique_ptr<Shader> mShader; // MRT�̃V�F�|�_
	RenderTarget mTargets[NUM];		 // GBuffer�̃����_�[�e�N�X�`��(������MRT�ŕ`�悷��)
	Vector2 mTargetSize;			 // GBuffer�̃T�C�Y
	UINT mSlot = 0;					 // �p�C�v���C���ɐݒ肵���X���b�g

public:// �֐�

	// �R���X�g���N�^
	GBuffer() = default;

	// �f�X�g���N�^
	virtual ~GBuffer() = default;

	// �������֐�
	void Initialize(float width, float height);
	
	// MRT�̐ݒ�
	void Activate();

	// �^�[�Q�b�g��߂�
	void Deactivate();

	// �p�C�v���C���Ƀe�N�X�`����ݒ�
	void SetTexture(UINT startSlot);
	
	RenderTarget* GetColorTarget() { return &mTargets[COLOR]; } // �J���[�̃����_�[�e�N�X�`�����擾
	Vector2 GetTargetSize() const { return mTargetSize; }       // GBuffer�̃T�C�Y������Ƃ�
};