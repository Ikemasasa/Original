#pragma once
#include <DirectXMath.h>

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "lib/Vector.h"

class CameraBase;

/*
	�G�t�F�N�g�̊Ǘ����s���N���X
	�F�X�ȂƂ��납��Ăяo�������̂�Singleton��ʂ�
*/
class EffectManager
{
	// �萔
	static const int SQUARE_MAX = 8000; // �K��
	static const int EFFECT_MAX = 256; // �K��
	static const int INST_MAX = 512;

private:// �ϐ�
	Effekseer::ManagerRef mManager;				  // �}�l�[�W��
	EffekseerRendererDX11::RendererRef mRenderer; // �����_���[

	Effekseer::EffectRef mEffects[EFFECT_MAX] = { nullptr }; // �G�t�F�N�g�̌��f�[�^
	int mEffectNum = 0;										 // ���f�[�^�̐�

	Effekseer::Handle mInstHandles[INST_MAX] = {-1}; // �G�t�F�N�g�̃C���X�^���X
	int mInstNum = 0;								 // �C���X�^���X�̐�

public:// �֐�

	// �R���X�g���N�^
	EffectManager();

	// �f�X�g���N�^
	~EffectManager();

	// �������֐�
	void Initialize();

	// �G�t�F�N�g�̌��f�[�^�쐬 / �߂�l : slot
	int Create(const EFK_CHAR* efkPath); 
	void Create(const EFK_CHAR* efkPath, int slot);

	// �X�V�֐�(�G�t�F�N�g�̃t���[���̍X�V�Ƃ����Ă�Ǝv��)
	void Update();

	// �`��֐�
	void Render(const Matrix& view, const Matrix& proj);

	// �G�t�F�N�g�Đ����X�g�ɒǉ� / �߂�l �F �C���X�^���X�n���h��
	int Play(const int& slot, const Vector3& pos, int startFrame = 0, float scale = 1.0f, float speed = 1.0f);

	// �G�t�F�N�g���~
	void Stop(const int instHandle);

	// �����̃G�t�F�N�g���Đ�����Ă��邩
	bool IsPlay(const int instHandle) const;
};