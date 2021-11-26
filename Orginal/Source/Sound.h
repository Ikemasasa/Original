#pragma once

// ���ʉ��Đ��N���X
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

public:// �֐�

    // ������
    static void Initialize();

    static void Play(Sound::Kind kind);    // �Đ�
    static void Stop(Sound::Kind kind);    // ��~
};