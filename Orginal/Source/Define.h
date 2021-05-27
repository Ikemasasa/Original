#pragma once

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
	static const float SCREEN_WIDTH;
	static const float SCREEN_HEIGHT;

	static const float FRAMERATE;
	static const float PI;
};