#pragma once

// 効果音再生クラス
class Sound
{
public:
    enum Kind
    {
        SELECT,
        SELECT_LONG,
        CANCEL,
        CURSOR_MOVE,
        MENU_OPEN,
        MENU_CLOSE,
        EQUIP,
        HEAL,
        BUFF_ATK,
        DEBUFF_DEF,
        BOMB,
        ATTACK_HIT,
        ENEMY_HIT,
        DEATH,
        COSMIC_MIST,
        NUM
    };

public:// 関数

    // 初期化
    static void Initialize();

    static void Play(Sound::Kind kind);    // 再生
    static void Stop(Sound::Kind kind);    // 停止
};