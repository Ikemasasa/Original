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
    ATTACK_HIT,
    ENEMY_HIT,
    NUM
};

class Audio
{
private:
    static const int MUSIC_NUM = 32;
    static const int MUSIC_INST_NUM = 2;
    static const int MUSIC_INST_MAIN_SLOT = 0;
    static const int MUSIC_INST_SUB_SLOT = 1;
    static constexpr float MUSIC_VOLUME_MAX = 0.5f;

    static const int SOUND_NUM = 32;
    static const int SOUND_INST_NUM = 32;

    static const int FADE_FRAME = 15;

    static std::unique_ptr<DirectX::AudioEngine>			mAudioEngine;

    //音楽用
    static std::unique_ptr<DirectX::SoundEffect>			mMusic[MUSIC_NUM];
    static std::unique_ptr<DirectX::SoundEffectInstance>	mMusicInst[MUSIC_INST_NUM];
    static float											mMusicVolume[MUSIC_INST_NUM];

    // 効果音用
    static std::unique_ptr<DirectX::SoundEffect>			mSound[SOUND_NUM];
    static std::unique_ptr<DirectX::SoundEffectInstance>	mSoundInst[SOUND_INST_NUM];
    static float											mSoundVolume[SOUND_INST_NUM];

private:
    static void MusicFade();
    static void UnLoadAll();

public:
    Audio() = default;
    ~Audio() = default;

    static void Initialize();
    static void Update();
    static void Release();

    static void LoadMusic(int slot, const wchar_t* waveFilename, float volume = 0.5f);
    static void LoadSound(int slot, const wchar_t* waveFilename, float volume = 1.0f);

    static void MusicPlay(int slot);   // 再生
    static void MusicStopAll();        // 全部ストップ
    static void MusicPause();          // 一時停止
    static void MusicResume();         // 再開

    static void SoundPlay(int slot);
    static void SoundStop(int slot);
};