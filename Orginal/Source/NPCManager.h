#pragma once
#include <DirectXMath.h>
#include <memory>
#include <vector>

#include "lib/Vector.h"

#include "NPC.h"
#include "NPCTalk.h"

class Shader;
class Sprite;

/*
	NPC�̃}�l�[�W���N���X
	NPC�̐����A�X�V�A�`��A��b����NPC�̕ۑ��Ƃ������Ă���
*/
class NPCManager
{
public: // �萔
	static constexpr float TALK_DIST = 8.0f; // ��b�\�ȍő勗��
	static constexpr float TALK_DOT = 0.5f;	 // ��b�\�ȍŒ���ϒl

private:
	static constexpr float ICON_RENDER_DIST = TALK_DIST * 2.0f; // ��b�\�̃A�C�R����\������ő勗��
	static constexpr float ICON_SCALE = 0.125f;					// �A�C�R���̃X�P�[��

private: // �ϐ�
	std::vector<std::unique_ptr<NPC>> mNPCs; // NPC�̃��X�g
	std::unique_ptr<Sprite> mTalkIcon;		 // ��b�\�A�C�R��
	Vector3 mPlayerPos = {};				 // �v���C���[�̍��W

public: // �֐�

	// �R���X�g���N�^
	NPCManager() = default;

	// �f�X�g���N�^
	~NPCManager() = default;

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Render(const Shader* shader, const Matrix& view, const Matrix& projection, const Vector4& lightDir);
	
	// UI��`��
	void RenderUI();

	// ���
	void Release();

	// NPC�𐶐�
	void Create(const int charaID);

	// NPC�̐����擾
	size_t GetNum() const { return mNPCs.size(); }

	// �����̔ԍ���NPC���擾
	NPC* GetNPC(size_t index) const { return mNPCs[index].get(); }

	// �v���C���[�̍��W��ݒ�
	void SetPlayerPos(const Vector3& pos) { mPlayerPos = pos; }
};