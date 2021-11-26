#pragma once

class GameManager
{
public:
	static float elapsedTime;  // 前回のフレームからの経過時間
	static bool bossSubdueFlag; // ボス討伐フラグ(もっとちゃんとしたやつにしたい)
};