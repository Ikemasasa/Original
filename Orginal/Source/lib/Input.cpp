#include <Windows.h>
#include <math.h>
#include "Input.h"

#pragma comment( lib,"xinput.lib")

Input::States Input::mStates[CONTROLLERS_MAX];
int Input::mButtonMap[BUTTON_MAX];
int Input::mKeyMap[BUTTON_MAX];


void Input::Initialize()
{
	int initMap[BUTTON_MAX] =
	{
		XINPUT_GAMEPAD_DPAD_UP,
		XINPUT_GAMEPAD_DPAD_DOWN,
		XINPUT_GAMEPAD_DPAD_LEFT,
		XINPUT_GAMEPAD_DPAD_RIGHT,
		XINPUT_GAMEPAD_START,
		XINPUT_GAMEPAD_BACK,
		XINPUT_GAMEPAD_LEFT_THUMB,
		XINPUT_GAMEPAD_RIGHT_THUMB,
		XINPUT_GAMEPAD_LEFT_SHOULDER,
		XINPUT_GAMEPAD_RIGHT_SHOULDER,
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y,
	};

	int initKeymap[BUTTON_MAX] =
	{
		VK_UP,		//UP,
		VK_DOWN,	//DOWN,
		VK_LEFT,	//LEFT,
		VK_RIGHT,	//RIGHT,
		VK_RETURN,	//START,
		VK_BACK,	//BACK,
		VK_SHIFT,	//LSTICK,
		VK_TAB,		//RSTICK,
		VK_CONTROL,	//LB,
		VK_SPACE,	//RB,
		'Z',		//A,
		'X',		//B,
		'C',		//X,
		'V',		//Y,
	};

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		mButtonMap[i] = initMap[i];
		mKeyMap[i] = initKeymap[i];
	}


}

void Input::Update()
{
	constexpr float STICK_VALUE_MAX = 32767.0f;
	constexpr float TRIGGER_VALUE_MAX = 255.0f;

	for (DWORD i = 0; i < CONTROLLERS_MAX; i++) 
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		DWORD dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS) 
		{
			//	左スティック
			{
				mStates[i].axisLX = state.Gamepad.sThumbLX / STICK_VALUE_MAX;
				mStates[i].axisLY = state.Gamepad.sThumbLY / STICK_VALUE_MAX;

				//	デッドゾーン
				float d = sqrtf(mStates[i].axisLX * mStates[i].axisLX + mStates[i].axisLY * mStates[i].axisLY);
				if (d < DEADZONE_L) {
					mStates[i].axisLX = 0;
					mStates[i].axisLY = 0;
				}
			}

			//	右スティック
			{
				mStates[i].axisRX = state.Gamepad.sThumbRX / STICK_VALUE_MAX;
				mStates[i].axisRY = state.Gamepad.sThumbRY / STICK_VALUE_MAX;
				//	デッドゾーン
				float d = sqrtf(mStates[i].axisRX * mStates[i].axisRX + mStates[i].axisRY * mStates[i].axisRY);
				if (d < DEADZONE_R) {
					mStates[i].axisRX = 0;
					mStates[i].axisRY = 0;
				}
			}

			//　トリガー
			{
				mStates[i].leftTrigger = state.Gamepad.bLeftTrigger / TRIGGER_VALUE_MAX;
				//	デッドゾーン
				if (mStates[i].leftTrigger < 0.2f) {
					mStates[i].leftTrigger = 0;
				}

				mStates[i].rightTrigger = state.Gamepad.bRightTrigger / TRIGGER_VALUE_MAX;
				//	デッドゾーン
				if (mStates[i].rightTrigger < 0.2f) {
					mStates[i].rightTrigger = 0;
				}
			}

			//	ボタン
			for (int b = 0; b < BUTTON_MAX; b++) {
				int old = mStates[i].buttons[b] & 0x01;
				if (state.Gamepad.wButtons & mButtonMap[b]) mStates[i].buttons[b] = 1;
				else  mStates[i].buttons[b] = 0;
				//	瞬間判定
				if (mStates[i].buttons[b] != old) mStates[i].buttons[b] |= 0x02;
			}
		}
		else
		{
			//	非接続の場合、０番のみキーボードで代用
			if (i == 0) 
			{
				//	左スティック
				{
					mStates[i].axisLX = 0;
					mStates[i].axisLY = 0;
					if (GetKeyState('W') < 0) mStates[i].axisLY += 1;
					if (GetKeyState('A') < 0) mStates[i].axisLX += -1;
					if (GetKeyState('S') < 0) mStates[i].axisLY += -1;
					if (GetKeyState('D') < 0) mStates[i].axisLX += 1;
					float d = sqrtf(mStates[i].axisLX * mStates[i].axisLX + mStates[i].axisLY * mStates[i].axisLY);
					if (d > 1.0f) {
						mStates[i].axisLX /= d;
						mStates[i].axisLY /= d;
					}
				}

				//	右スティック
				{
					mStates[i].axisRX = 0;
					mStates[i].axisRY = 0;
					if (GetKeyState('I') < 0) mStates[i].axisRY += 1;
					if (GetKeyState('J') < 0) mStates[i].axisRX += -1;
					if (GetKeyState('K') < 0) mStates[i].axisRY += -1;
					if (GetKeyState('L') < 0) mStates[i].axisRX += 1;
					float d = sqrtf(mStates[i].axisRX * mStates[i].axisRX + mStates[i].axisRY * mStates[i].axisRY);
					if (d > 1.0f) {
						mStates[i].axisRX /= d;
						mStates[i].axisRY /= d;
					}
				}

				//	トリガー
				{
					mStates[i].leftTrigger = 0;
					mStates[i].rightTrigger = 0;
					if (GetKeyState('Q') < 0) mStates[i].leftTrigger = 1;
					if (GetKeyState('E') < 0) mStates[i].rightTrigger = 1;
				}

				//	ボタン
				for (int b = 0; b < 16; b++) {
					int old = mStates[i].buttons[b] & 0x01;
					if (GetKeyState(mKeyMap[b]) < 0) mStates[i].buttons[b] = 1;
					else  mStates[i].buttons[b] = 0;
					//	瞬間判定
					if (mStates[i].buttons[b] != old) mStates[i].buttons[b] |= 0x02;
				}
			}
		}
	}
}

float Input::GetAxisX(int index)
{
	return mStates[index].axisLX;
}

float Input::GetAxisY(int index)
{
	return mStates[index].axisLY;
}

float Input::GetAxisRX(int index)
{
	return mStates[index].axisRX;
}

float Input::GetAxisRY(int index)
{
	return mStates[index].axisRY;
}

int Input::GetButton(int index, BUTTON button)
{
	return mStates[index].buttons[button];
}

bool Input::GetButtonState(int index, BUTTON button)
{
	return GetButton(index, button) == PRESSED;
}

bool Input::GetButtonTrigger(int index, BUTTON button)
{
	return GetButton(index, button) == JUST_PRESSED;
}

bool Input::GetButtonRelase(int index, BUTTON button)
{
	return GetButton(index, button) == JUST_RELEASED;
}

void Input::SetVibration(int index, float left, float right)
{
	if (left < 0) left = 0;
	if (left > 1) left = 1;
	if (right < 0) right = 0;
	if (right > 1) right = 1;

	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = (WORD)(left * 65535); // use any value between 0-65535 here
	vibration.wRightMotorSpeed = (WORD)(right * 65535); // use any value between 0-65535 here
	XInputSetState(index, &vibration);
}
