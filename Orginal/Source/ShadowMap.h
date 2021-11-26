#pragma once
#include <memory>

#include "GaussianBlur.h"

#include "lib/ConstantBuffer.h"
#include "lib/RenderTarget.h"
#include "lib/Shader.h"
#include "lib/Vector.h"
#include "lib/Matrix.h"

// �V���h�E�}�b�v�𐶐��A�ݒ肷��N���X
class ShadowMap
{
	// �萔
	static constexpr float SHADOWMAP_X = 4098; // �V���h�E�}�b�v�̃e�N�X�`���T�C�Y
	static constexpr float SHADOWMAP_Y = 4098; // �V���h�E�}�b�v�̃e�N�X�`���T�C�Y
	static constexpr float BLUR_STRENGTH = 20; // VSM�̃u���[�̋���

private: // �ϐ�
	RenderTarget mShadowMap;		 // �V���h�E�}�b�v�̃����_�[�e�N�X�`��
	RenderTarget mVarianceShadowMap; // VSM�̃����_�[�e�N�X�`��
	GaussianBlur mGaussianBlur;		 // �K�E�X�u���[��������N���X
	std::unique_ptr<Shader> mShader; // �V���h�E�}�b�v�V�F�[�_�[

	struct CBShadow
	{
		Matrix shadowVP;
	};
	ConstantBuffer mConstBuffer;	 // �萔�o�b�t�@

	Matrix mShadowProj;	// �V���h�E�}�b�v�̗p�̃v���W�F�N�V�����s��
	Vector3 mTarget;	// �V���h�E�}�b�v�p�̃J�����̃^�[�Q�b�g

public:
	ShadowMap();
	~ShadowMap();

	void Initialize();
	void Activate(const Vector3& target, const Vector3& dir, float maxDistance =  170.0f);
	void Deactivate();
	void SetTexture(UINT textureSlot);
	Shader* GetShader()const { return mShader.get(); }
};

