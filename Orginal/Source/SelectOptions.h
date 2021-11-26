#pragma once
#include <memory>
#include <string>

#include "lib/Font.h"
#include "lib/Vector.h"

class Sprite;

// �I������I�ԃN���X
class SelectOptions
{
	// �萔
	static constexpr float TITLE_STR_OFFSET_Y = 5.0f;  // �I�����̃^�C�g���̃I�t�Z�b�gY
	static constexpr float OPTION_STR_OFFSET_X = 7.0f; // �I�����̃I�t�Z�b�gX
	static constexpr float SELECT_OFFSET_X = 5.0f;	   // �I���摜�̃I�t�Z�b�gX

private: // �ϐ�
	std::unique_ptr<Sprite> mOptionTitle;  // �I�����̃^�C�g���\���ꏊ�̉摜
	std::unique_ptr<Sprite> mOption;	   // �I�����̕\���ꏊ�̉摜
	std::unique_ptr<Sprite> mOptionEnd;	   // �摜�̈�ԉ�
	std::unique_ptr<Sprite> mOptionSelect; // �I���摜
	
	Font mFont;							   // �t�H���g
	std::wstring mStrTitle;				   // �^�C�g���̃e�L�X�g
	std::vector<std::wstring> mStrOptions; // �I�����̃e�L�X�g
	int mOptionIndex = 0;				   // ���ݑI��ł�I����

public:// �֐�

	// �R���X�g���N�^
	SelectOptions() = default;

	// �f�X�g���N�^
	~SelectOptions() = default;

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Render(const Vector2& leftTop, bool isStrsClear = true);

	// �I�����̃^�C�g����ݒ�
	void AddTitle(const wchar_t* str);

	// �I������ǉ�
	void AddOption(const wchar_t* str);

	// �I��ł�I�������擾
	int GetIndex() const { return mOptionIndex; }
};