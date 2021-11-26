#pragma once

// å¯â âπçƒê∂ÉNÉâÉX
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

public:// ä÷êî

    // èâä˙âª
    static void Initialize();

    static void Play(Sound::Kind kind);    // çƒê∂
    static void Stop(Sound::Kind kind);    // í‚é~
};