#pragma once
#include "MenuBase.h"

#include <vector>

#include "HealthGauge.h"
#include "ItemSelect.h"
#include "MenuCharacterSelect.h"
#include "SelectOptions.h"

class ItemInventory;
class PlayerManager;

/*
	���j���[�̃A�C�e���g�p���
*/
class MenuItem : public MenuBase
{
	// �萔
	static constexpr float BOARD_OFFSET_X = 300.0f; // �A�C�e���I���{�[�h��X���W
	static constexpr float BOARD_OFFSET_Y = 150.0f; // �A�C�e���I���{�[�h��Y���W

	static constexpr float OPTION_BOARD_X = 400.0f; // �I�����̃{�[�h��X���W
	static constexpr float OPTION_BOARD_Y = 250.0f; // �I�����̃{�[�h��Y���W

	static constexpr float HEALTH_BOARD_X = 900.0f; // �L�����N�^�[�̃w���X�\���{�[�h��X���W
	static constexpr float HEALTH_BOARD_Y = 100.0f; // �L�����N�^�[�̃w���X�\���{�[�h��Y���W

private:// �ϐ�
	ItemInventory* mInventory; // �A�C�e���C���x���g���̎Q��

	int mItemIndex = 0;					   // �I���ꏊ
	bool mIsUseFieldItem = false;		   // �t�B�[���h�g�p�A�C�e�����g����
	MenuCharacterSelect mCharacterSelect;  // �Q�Ɛ�̃L�����N�^�[�؂�ւ��N���X
	std::vector<HealthGauge> mHealthGauge; // �L�����N�^�[�̃w���X�\���N���X
	ItemSelect mItemSelect;				   // �A�C�e���I���N���X
	SelectOptions mSelectOptions;		   // �I�����I���N���X

private: // �֐�

	// �񕜃A�C�e�����g�p
	void UseHealItem(const PlayerManager* plm, const int itemID);
	
	// �t�B�[���h�g�p�A�C�e�����g�p
	void UseFieldUseItem(const int itemID);

public:

	// �R���X�g���N�^
	MenuItem() = default;

	// �f�X�g���N�^
	~MenuItem() = default;
	
	// ������
	void Initialize(const PlayerManager* plm) override;
	
	// �X�V
	Select Update(PlayerManager* plm) override;
	
	// �`��
	void Render() override;
	
	// ���
	void Release() override;
};