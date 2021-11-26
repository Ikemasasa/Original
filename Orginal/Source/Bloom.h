#pragma once
#include <memory>

#include "Define.h"

class GaussianBlur;
class Shader;
class RenderTarget;

// RenderTarget����u���[�������N���X
class Bloom
{
	// �萔
	// �X�N���[�� / 4���ŏ��̃o�b�t�@�T�C�Y
	static const int FIRST_BUFFER_SIZE_X = static_cast<int>(Define::SCREEN_WIDTH / 4);
	static const int FIRST_BUFFER_SIZE_Y = static_cast<int>(Define::SCREEN_HEIGHT / 4);
	static const int BUFFER_NUM = 4;			   // �o�b�t�@�̐�
	static constexpr float BLUR_INTENSITY = 50.0f; // �u���[�̋���

private: // �ϐ�
	std::unique_ptr<GaussianBlur> mTarget[BUFFER_NUM]; // �u���[���������e�N�X�`��
	std::unique_ptr<RenderTarget> mBrightness;		   // �P�x���o�e�N�X�`��
	std::unique_ptr<Shader> mExtractBrightness;		   // �P�x���o�V�F�[�_

private: // �֐�

	// ��������P�x���o���āA�u���[���������e�N�X�`�������
	void MakeBuffer(RenderTarget* tex);

	// �u���[���������e�N�X�`������������
	void Compose(RenderTarget* tex);

public:
	// �R���X�g���N�^
	Bloom();

	// �f�X�g���N�^
	~Bloom();

	// �������֐�
	void Initialize();

	// �u���[�����s�֐�(�����̃e�N�X�`��������)
	void Execute(RenderTarget* tex);
};