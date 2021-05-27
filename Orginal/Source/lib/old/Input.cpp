#include "Input.h"
#include <Windows.h>

INT64 Input::mState = 0;
INT64 Input::mTrigger = 0;
INT64 Input::mRelease = 0;
bool Input::mEnable = true;

void Input::Update()
{
	INT64 oldState = mState;
	mState = 0;
	mTrigger = 0;
	mRelease = 0;

	if (!mEnable) return;

	if (GetKeyState(VK_UP)      < 0) mState |= KeyCode::UP;
	if (GetKeyState(VK_DOWN)    < 0) mState |= KeyCode::DOWN;
	if (GetKeyState(VK_RIGHT)   < 0) mState |= KeyCode::RIGHT;
	if (GetKeyState(VK_LEFT)    < 0) mState |= KeyCode::LEFT;
	if (GetKeyState(VK_RETURN)  < 0) mState |= KeyCode::RETURN;
	if (GetKeyState(VK_SPACE)   < 0) mState |= KeyCode::SPACE;
	if (GetKeyState(VK_CONTROL) < 0) mState |= KeyCode::CTRL;
	if (GetKeyState(VK_SHIFT)   < 0) mState |= KeyCode::SHIFT;
	if (GetKeyState(VK_TAB)     < 0) mState |= KeyCode::TAB;
	if (GetKeyState('Q')        < 0) mState |= KeyCode::Q;
	if (GetKeyState('W')        < 0) mState |= KeyCode::W;
	if (GetKeyState('E')        < 0) mState |= KeyCode::E;
	if (GetKeyState('R')		< 0) mState |= KeyCode::R;
	if (GetKeyState('T')		< 0) mState |= KeyCode::T;
	if (GetKeyState('Y')		< 0) mState |= KeyCode::Y;
	if (GetKeyState('U')		< 0) mState |= KeyCode::U;
	if (GetKeyState('I')		< 0) mState |= KeyCode::I;
	if (GetKeyState('O')		< 0) mState |= KeyCode::O;
	if (GetKeyState('P')		< 0) mState |= KeyCode::P;
	if (GetKeyState('A')		< 0) mState |= KeyCode::A;
	if (GetKeyState('S')		< 0) mState |= KeyCode::S;
	if (GetKeyState('D')		< 0) mState |= KeyCode::D;
	if (GetKeyState('F')		< 0) mState |= KeyCode::F;
	if (GetKeyState('G')		< 0) mState |= KeyCode::G;
	if (GetKeyState('H')		< 0) mState |= KeyCode::H;
	if (GetKeyState('J')		< 0) mState |= KeyCode::J;
	if (GetKeyState('K')		< 0) mState |= KeyCode::K;
	if (GetKeyState('L')		< 0) mState |= KeyCode::L;
	if (GetKeyState('Z')		< 0) mState |= KeyCode::Z;
	if (GetKeyState('X')		< 0) mState |= KeyCode::X;
	if (GetKeyState('C')		< 0) mState |= KeyCode::C;
	if (GetKeyState('V')		< 0) mState |= KeyCode::V;
	if (GetKeyState('B')		< 0) mState |= KeyCode::B;
	if (GetKeyState('N')		< 0) mState |= KeyCode::N;
	if (GetKeyState('M')		< 0) mState |= KeyCode::M;
	if (GetKeyState('1')		< 0) mState |= KeyCode::ALPHA1;
	if (GetKeyState('2')		< 0) mState |= KeyCode::ALPHA2;
	if (GetKeyState('3')		< 0) mState |= KeyCode::ALPHA3;
	if (GetKeyState('4')		< 0) mState |= KeyCode::ALPHA4;
	if (GetKeyState('5')		< 0) mState |= KeyCode::ALPHA5;
	if (GetKeyState('6')		< 0) mState |= KeyCode::ALPHA6;
	if (GetKeyState('7')		< 0) mState |= KeyCode::ALPHA7;
	if (GetKeyState('8')		< 0) mState |= KeyCode::ALPHA8;
	if (GetKeyState('9')		< 0) mState |= KeyCode::ALPHA9;
	if (GetKeyState('0')		< 0) mState |= KeyCode::ALPHA0;

	mTrigger = (~oldState) & mState;
	mRelease = oldState & (~mState);
}
