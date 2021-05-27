#pragma once
#include <windows.h>

// ì¸óÕÇ´Å[Ç—Ç¡Ç∆
struct KeyCode
{
	static const INT64 UP	  = 0x00000000000001;
	static const INT64 DOWN   = 0x00000000000002;
	static const INT64 LEFT   = 0x00000000000004;
	static const INT64 RIGHT  = 0x00000000000008;
	static const INT64 RETURN = 0x00000000000010;
	static const INT64 SPACE  = 0x00000000000020;
	static const INT64 CTRL   = 0x00000000000040;
	static const INT64 SHIFT  = 0x00000000000080;
	static const INT64 TAB    = 0x00000000000100;
	static const INT64 Q	  = 0x00000000000200;
	static const INT64 W      = 0x00000000000400;
	static const INT64 E      = 0x00000000000800;
	static const INT64 R      = 0x00000000001000;
	static const INT64 T	  = 0x00000000002000;
	static const INT64 Y      = 0x00000000004000;
	static const INT64 U      = 0x00000000008000;
	static const INT64 I      = 0x00000000010000;
	static const INT64 O	  = 0x00000000020000;
	static const INT64 P      = 0x00000000040000;
	static const INT64 A      = 0x00000000080000;
	static const INT64 S      = 0x00000000100000;
	static const INT64 D	  = 0x00000000200000;
	static const INT64 F      = 0x00000000400000;
	static const INT64 G      = 0x00000000800000;
	static const INT64 H      = 0x00000001000000;
	static const INT64 J	  = 0x00000002000000;
	static const INT64 K      = 0x00000004000000;
	static const INT64 L      = 0x00000008000000;
	static const INT64 Z      = 0x00000010000000;
	static const INT64 X	  = 0x00000020000000;
	static const INT64 C      = 0x00000040000000;
	static const INT64 V      = 0x00000080000000;
	static const INT64 B      = 0x00000100000000;
	static const INT64 N	  = 0x00000200000000;
	static const INT64 M      = 0x00000400000000;
	static const INT64 ALPHA1 = 0x00000800000000;
	static const INT64 ALPHA2 = 0x00001000000000;
	static const INT64 ALPHA3 = 0x00002000000000;
	static const INT64 ALPHA4 = 0x00004000000000;
	static const INT64 ALPHA5 = 0x00008000000000;
	static const INT64 ALPHA6 = 0x00010000000000;
	static const INT64 ALPHA7 = 0x00020000000000;
	static const INT64 ALPHA8 = 0x00040000000000;
	static const INT64 ALPHA9 = 0x00080000000000;
	static const INT64 ALPHA0 = 0x00100000000000;
								 
};

class Input
{
private:
	Input() = default;
	~Input() = default;
	
	static INT64 mState;
	static INT64 mTrigger;
	static INT64 mRelease;
	static bool mEnable;

public:
	static void Update();
	static INT64 State()				 { return mState; }
	static bool  State(INT64 keyIndex)	 { return mState & keyIndex; }
	static INT64 Trigger()				 { return mTrigger; }
	static bool  Trigger(INT64 keyIndex) { return mTrigger & keyIndex; }
	static INT64 Release()				 { return mRelease; }
	static bool  Release(INT64 keyIndex) { return mRelease & keyIndex; }
	static void Enable()				 { mEnable = true; }
	static void Disable()				 { mEnable = false; }
};