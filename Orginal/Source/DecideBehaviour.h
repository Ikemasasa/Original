#pragma once
#include "IDecideCommand.h"

#include "lib/Font.h"
#include "lib/TVector.h"

class Sprite;

/*
	�퓬���̍s�������肷��N���X
	�U���A�X�L���A�h��etc... �I��
*/
class DecideBehaviour : public IDecideCommand
{
	// �萔
	static const int COMMAND_MIN_X = 0;	   // �R�}���hX�̍Œ�l
	static const int COMMAND_MAX_X = 2;    // �R�}���hX�̍ō��l
	static const int COMMAND_MIN_Y = 0;    // �R�}���hY�̍Œ�l
	static const int COMMAND_MAX_Y = 2;    // �R�}���hY�̍ō��l
	static const int COMMAND_NUM = 5;	   // �R�}���h�̐�
	static const int COMMAND_VERTICAL = 3; // �R�}���h�̏c�̐�

	static constexpr float ICON_SIZE = 256.0f;	// �A�C�R���̎��T�C�Y
	static constexpr float ICON_SCALE = 0.25f;  // �A�C�R���̃X�P�[��
	static constexpr float ICON_SELECT_X = ICON_SIZE * 5; // �A�C�R���̑I���摜�̃e�N�X�`�����W(px)
	static constexpr float ICON_BASE_X   = ICON_SIZE * 6; // �A�C�R���̉��n�̃e�N�X�`�����W(px)

private:// �ϐ�
	TVector2<int> mCommandIndex;			   // �I�𒆂̃R�}���h
	std::unique_ptr<Sprite> mIcons;			   // �A�C�R���̉摜
	std::unique_ptr<Sprite> mCommandNameBoard; // �R�}���h����\������{�[�h�̉摜
	Font mFont;								   // �t�H���g�N���X

public: // �֐�
	
	// �R���X�g���N�^
	DecideBehaviour();

	// �f�X�g���N�^
	~DecideBehaviour() = default;

	// ������
	void Initialize(const BattleCharacterManager* bcm) override;
	
	// �X�V
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	
	// �`��
	void Render() override;

	// �p�����[�^���Z�b�g
	void ResetParam() override;
};