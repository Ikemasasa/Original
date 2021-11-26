#pragma once

// 

/*
    BGMを管理するクラス
    
    すでに流れている状態からPlay関数を呼ぶと、
    今流れているものをフェードアウトさせて新しいものをフェードインするようにしてる
*/
class Music
{
public:
	enum Kind
	{
        TITLE,
        FIELD_REMAINS,
        BATTLE,
        BOSS_BATTLE,
        RESULT,
        BOSS_RESULT,
        NUM
	};

public: // 関数

    // 初期化関数
    static void Initialize();

    // 再生
    static void Play(Music::Kind kind);    
    
    // 全部ストップ
    static void StopAll();

    // 一時停止
    static void Pause();   

    // 再開
    static void Resume();  
};