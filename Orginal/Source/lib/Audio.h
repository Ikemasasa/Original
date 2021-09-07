#pragma once
#include "DirectXTK/Inc/Audio.h"

enum class Music
{
    TITLE,
    FIELD_REMAINS,
    BATTLE,
    BOSS_BATTLE,
    RESULT,
    BOSS_RESULT,
    NUM
};

enum class Sound
{
    SELECT,
    SELECT_LONG,
    CANCEL,
    CURSOR_MOVE,
    MENU_OPEN,
    MENU_CLOSE,
    EQUIP,
    HEAL,
    BUFF,
    DEBUFF,
    BOMB,
    NUM
};

class Audio
{
private:
    static const int MUSIC_NUM = 32;
    static const int SOUND_NUM = 32;

    std::unique_ptr<DirectX::AudioEngine>			mAudioEngine;

    //���y�p
    std::unique_ptr<DirectX::SoundEffect>			mMusic[MUSIC_NUM];
    std::shared_ptr<DirectX::SoundEffectInstance>	mMusicInst[MUSIC_NUM];
    float											mMusicVolume[MUSIC_NUM];

    // ���ʉ��p
    std::unique_ptr<DirectX::SoundEffect>			mSound[SOUND_NUM];
    std::shared_ptr<DirectX::SoundEffectInstance>	mSoundInst[SOUND_NUM];
    float											mSoundVolume[SOUND_NUM];

public:
    Audio();
    ~Audio();

    void Initialize();

    void LoadMusic(int slot, const wchar_t* waveFilename, float volume = 0.5f);
    void UnLoadMusic(int slot);

    void LoadSound(int slot, const wchar_t* waveFilename, float volume = 1.0f);
    void UnLoadSound(int slot);

    void MusicStopAll(); // �S���X�g�b�v
    void MusicPlay(int slot);   // �Đ�
    void MusicStop(int slot);   // ��~
    void MusicPause(int slot);  // �ꎞ��~
    void MusicResume(int slot); // �ĊJ
    void MusicSetVolume(int slot, float volume); //���ʐݒ�

    void SoundPlay(int slot);
    void SoundStop(int slot);

    static Audio& GetInstance()
    {
        static Audio instance;
        return instance;
    }
};

#define AUDIO (Audio::GetInstance())