#pragma once
#include "MenuBase.h"

#include <memory>
#include <vector>
#include <string>

#include "lib/Vector.h"
#include "lib/Font.h"

#include "HealthGauge.h"

class Sprite;

// �ǂ̃��j���[��ʂɍs�����̑I���N���X
class MenuSelect : public MenuBase
{
	// �萔
	static constexpr float BOARD_POS_X = 100.0f;	// �e���j���\��ʂ̃e�L�X�g�̃{�[�h��X���W
	static constexpr float BOARD_POS_Y = 75.0f;		// �e���j���\��ʂ̃e�L�X�g�̃{�[�h��Y���W
	static constexpr float BOARD_OFFSET_Y = 15.0f;  // �e���j���\��ʂ̃e�L�X�g�̃{�[�h��Y���W�̊��o
	static const int STR_OFFSET_X = 5;				// �e���j���\��ʂ̃e�L�X�g�̃{�[�h�����X�ʒu
	static const int STR_OFFSET_Y = 5;				// �e���j���\��ʂ̃e�L�X�g�̃{�[�h�����Y�ʒu
	static constexpr float MOVE_MAX = 30.0f;		// �J�n���o�̓����ő勗��(data.pos + MOVE_MAX �̈ʒu���ŏ��Ɉʒu�ɂȂ�)

	static constexpr float HEALTH_BOARD_POS_X = 800.0f; //�̗̓Q�[�W�̃{�[�h��X���W
	static constexpr float HEALTH_BOARD_POS_Y = 75.0f; //�̗̓Q�[�W�̃{�[�h��Y���W
	static constexpr float FADE = 0.15f; // �t�F�[�h�̃X�s�[�h


private: // �ϐ�
	struct Data
	{
		Vector2 pos;
		float moveX;
	};

	std::unique_ptr<Sprite> mStrBoard;		   // �e���j���\��ʂ̃e�L�X�g�̃{�[�h�̉摜
	std::unique_ptr<Sprite> mStrSelect;		   // �e���j���\��ʂ̃e�L�X�g�̃{�[�h�̑I���摜
	std::vector<HealthGauge> mHealthGauge;	   // �w���X�Q�[�W�쐬�N���X
	std::vector<Data> mDatas;				   //  �e���j���\��ʂ̃e�L�X�g�̃{�[�h�̍��W�f�[�^
	Font mFont;								   // �t�H���g�N���X

public:// �֐�

	// �R���X�g���N�^
	MenuSelect() = default;

	// �f�X�g���N�^
	~MenuSelect() = default;

	// �J�n���o
	void BeginAnimation();

	// ������
	void Initialize(const PlayerManager* plm) override;
	
	// �X�V
	Select Update(PlayerManager* plm) override;
	
	// �`��
	void Render() override;
	
	// ���
	void Release() override;
};