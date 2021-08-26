#pragma once
#include "lib/Vector.h"

/* メモ
* ヘッダーのインクルード順
* １，(cppの場合) 対応するヘッダ
* ２，<>でインクルードできるもの
* ３，自作ライブラリのへっだ
* ４，その他
* 
* １，２，３，４の間に空白を入れること
*/



struct Define final
{
	static constexpr float SCREEN_WIDTH = 1280.0f;
	static constexpr float SCREEN_HEIGHT = 720.0f;

	static constexpr float FRAMERATE = 60.0f;
	static constexpr float PI = 3.1416f;

	static const Vector4 FONT_COLOR;
	static const Vector4 FONT_BUFF_COLOR;
};