#pragma once
#include <memory>

#include "lib/Font.h"
#include "lib/Sprite.h"

#include "StatusData.h"

/*
	�L�����N�^�[��HP,MP���Q�[�W������N���X
	���g�͕�����摜�̈ʒu�����ŁA�͂���߂���Ȃ̂Ō��Ȃ��ł�������
*/
class HealthGauge
{
public: // �ϐ�

	// �ǂ��𒆐S�Ƃ��邩
	enum Center
	{
		LEFTTOP,
		MIDTOP,
		RIGHTTOP,
		LEFTMID,
		MID,
		RIGHTMID,
		LEFTBOTTOM,
		MIDBOTTOM,
		RIGHTBOTTOM
	};

private:
	enum BUFF
	{
		ATK_UP,
		ATK_DOWN,
		DEF_UP,
		DEF_DOWN,

		BUFF_NUM
	};

private: // �萔
	static const int FONT_SIZE = 22;							// �t�H���g�T�C�Y
	static const int FONT_WEIGHT = 22;							// �t�H���g�E�F�C�g
	static constexpr float SCALE_X = 0.45f;						// �摜�̃X�P�[��X
	static constexpr float SCALE_Y = 0.30f;						// �摜�̃X�P�[��Y
	static constexpr float GAUGE_OFFSET = 4 * SCALE_X;			// �Q�[�W�摜�̃I�t�Z�b�gX
	static constexpr float GAUGE_FIRST_OFFSET_Y = 55 * SCALE_Y; // �Q�[�W�摜�̃I�t�Z�b�gY
	static constexpr float HP_STR_OFFSET_X = 15 * SCALE_X;		// HP�̃t�H���g�̃I�t�Z�b�gX
	static constexpr float SELECT_GAUGE_ALPHA = 0.6f;		    // �I���摜�̃A���t�@�l

	static constexpr float BUFF_ICON_SCALE = 0.15f;								    // �o�t�A�C�R���̃X�P�[��
	static constexpr float BUFF_ICON_SIZE = 128;								    // �o�t�A�C�R���̎��T�C�Y
	static constexpr float BUFF_ICON_SCALE_SIZE = BUFF_ICON_SIZE * BUFF_ICON_SCALE; // �o�t�A�C�R���̃X�P�[���𔽉f�������T�C�Y

private: // �ϐ�
	
	// �摜�֌W
	std::unique_ptr<Sprite> mBaseBoard;   // �S�̂̉摜
	std::unique_ptr<Sprite> mGauge;		  // �Q�[�W�̉摜 
	std::unique_ptr<Sprite> mGaugeBoard;  // �Q�[�W����ꂽ���ɏo�闠���̉摜
	std::unique_ptr<Sprite> mGaugeSelcet; // �Q�[�W�I���摜
	std::unique_ptr<Sprite> mBuffIcons;   // �o�t�̃A�C�R���摜
	Vector2 mCenter;   // �摜�̒��S
	Vector2 mPos;	   // �摜�̍��W
	float mRGB = 0.0f; // �摜��RGB����

	// �Q�[�W�֌W
	Font mFont;							 // �t�H���g
	bool mIsInitialize = false;			 // �������������̃t���O
	bool mBuffFlags[BUFF_NUM];			 // �ǂ̃o�t���������Ă邩�̃t���O
	std::wstring mName;	// �L�����̖��O
	int mCurrentHP;		// �L������HP
	float mHPRate;// �L������HP����
	int mCurrentMP;// �L������MP
	float mMPRate;	// �L������MP����

private: // �֐�
	
	// �X�e�[�^�X�̕ϐ��ɐݒ肷��
	void SetStatusValue(const Status& status);

	// �摜�̒��S�����W�ɕϊ����Đݒ肷��
	void DecideCenter(const Center& center);

	// �t�H���g�̕`�揀��
	void RenderSetFont();

public:
	
	// �������֐�
	void Initialize();

	// �X�e�[�^�X��ݒ�
	void Set(const Status& status, const Vector2& pos, Center center, float rgb = 1.0f);
	void Set(const Status& status, const Vector3& pos, Center center, float rgb = 1.0f);

	// �`��
	//void Render();
	void Render(bool isSelectRender = false);

	// �I���摜�`��
	void RenderSelect();

	float GetWidth() const { return mBaseBoard->GetSize().x * SCALE_X; }  // �摜�̉������擾
	float GetHeight() const { return mBaseBoard->GetSize().y * SCALE_Y; } // �摜�̏c�����擾
};