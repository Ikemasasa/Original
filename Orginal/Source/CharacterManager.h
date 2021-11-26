#pragma once
#include <DirectXMath.h>
#include <memory>

#include "EnemyManager.h"
#include "NPCManager.h"
#include "NPCTalk.h"
#include "PlayerManager.h"

class Character;
class Player;
class Shader;

// �L�����N�^�[�̃}�l�[�W���N���X
class CharacterManager
{
public: // �萔




private: // �ϐ�
	std::unique_ptr<PlayerManager> mPlayerManager; // �v���C���[�}�l�[�W��
	std::unique_ptr<EnemyManager> mEnemyManager;   // �G�l�~�[�}�l�[�W��
	std::unique_ptr<NPCManager> mNPCManager;	   // NPC�}�l�[�W��
	std::unique_ptr<NPCTalk> mNPCTalk;			   // NPC�Ɖ�b���鏈��������N���X
	Enemy* mHitEnemy = nullptr;					   // �t�B�[���h�ŐړG�����G�̃V���{��

private: // �֐�
	
	// ��������
	void Collide(Character* a, Character* b);

	// �����o��
	void CollideObject(Character* move, Character* object);

public: 

	// �R���X�g���N�^
	CharacterManager();
	
	// �f�X�g���N�^
	~CharacterManager() = default;

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);
	
	// UI��`��
	void RenderUI();

	// NPC�Ƃ̏Փ˔���
	void CollideNPC();

	// �G�Ƃ̏Փ˔���
	void CollideEnemy();

	// NPC�Ɖ�b���ł��邩�`�F�b�N����
	void TalkCheck();

	// �ړG�����G�����邩�ǂ������擾
	Enemy* IsHitEnemy() const { return mHitEnemy; }

	// �ݒ肵���G�̕ϐ������Z�b�g
	void ResetHitEnemy() { mHitEnemy = nullptr; }

	// �t�B�[���h�œ������v���C���[���擾
	Player* GetMovePlayer() const { return mPlayerManager->GetMovePlayer(); }

	// �v���C���[�}�l�[�W�����擾
	PlayerManager* GetPlayerManager() const { return mPlayerManager.get(); }

	// �G�l�~�[�}�l�[�W�����擾
	EnemyManager* GetEnemyManager() const { return mEnemyManager.get(); }
};