#pragma once
#include <Windows.h>
#include <XInput.h>

class Input
{
	static const int CONTROLLERS_MAX = 4;
	static const int BUTTON_MAX = 16;

	static constexpr float DEADZONE_L = 0.2f;
	static constexpr float DEADZONE_R = 0.2f;

	//	�{�^���A�L�[�̃}�b�v
	static int mButtonMap[BUTTON_MAX];
	static int mKeyMap[BUTTON_MAX];

	//	���͏�ԕۑ��p
	struct States {
		float axisLX;
		float axisLY;
		float axisRX;
		float axisRY;

		float leftTrigger;
		float rightTrigger;

		int buttons[BUTTON_MAX];
	};
	static States mStates[CONTROLLERS_MAX];

public:
	//	�{�^����
	enum BUTTON {
		UP = 0,
		DOWN,
		LEFT,
		RIGHT,
		START,
		BACK,
		LSTICK,
		RSTICK,
		LB,
		RB,
		A,
		B,
		X,
		Y,
	};

	//	�{�^���̏��
	static const int RELEASED = 0;		// ����
	static const int PRESSED = 1;		// ����
	static const int JUST_RELEASED = 2;	// �������u��
	static const int JUST_PRESSED = 3;	// �������u��


	static void Initialize();
	static void Update();

	//	��Ԏ擾
	static float GetAxisX(int index = 0);
	static float GetAxisY(int index = 0);
	static float GetAxisRX(int index = 0);
	static float GetAxisRY(int index = 0);
	static int GetButton(int index, BUTTON button);

	static bool GetButtonState(int index, BUTTON button);
	static bool GetButtonTrigger(int index, BUTTON button);
	static bool GetButtonRelase(int index, BUTTON button);

	//	�U���ݒ�
	static void SetVibration(int index, float left, float right);
};

