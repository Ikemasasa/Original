#pragma once
#include <Windows.h>
#include <XInput.h>

class Input
{
	static const int CONTROLLERS_MAX = 4;
	static const int BUTTON_MAX = 16;

	static constexpr float DEADZONE_L = 0.2f;
	static constexpr float DEADZONE_R = 0.2f;

	//	ボタン、キーのマップ
	static int mButtonMap[BUTTON_MAX];
	static int mKeyMap[BUTTON_MAX];

	//	入力状態保存用
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
	//	ボタン名
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

	//	ボタンの状態
	static const int RELEASED = 0;		// 離し
	static const int PRESSED = 1;		// 押し
	static const int JUST_RELEASED = 2;	// 離した瞬間
	static const int JUST_PRESSED = 3;	// 押した瞬間


	static void Initialize();
	static void Update();

	//	状態取得
	static float GetAxisX(int index = 0);
	static float GetAxisY(int index = 0);
	static float GetAxisRX(int index = 0);
	static float GetAxisRY(int index = 0);
	static int GetButton(int index, BUTTON button);

	static bool GetButtonState(int index, BUTTON button);
	static bool GetButtonTrigger(int index, BUTTON button);
	static bool GetButtonRelase(int index, BUTTON button);

	//	振動設定
	static void SetVibration(int index, float left, float right);
};

