#pragma once
#include "DirectXTK/Inc/Audio.h"

// そのうちopenalに変えよう
// 後、music, soundは別クラスにした方がいいかもしれない 

enum class Music
{
    TITLE,
    FIELD_REMAINS,
    BATTLE,
    RESULT,
    NUM
};

enum class Sound
{
    SELECT,
    SELECT_LONG,
    CANCEL,
    CURSOR_MOVE,
    HEAL,
    NUM
};

class Audio
{
private:
    static const int MUSIC_NUM = 32;
    static const int SOUND_NUM = 32;

    std::unique_ptr<DirectX::AudioEngine>			mAudioEngine;

    //音楽用
    std::unique_ptr<DirectX::SoundEffect>			mMusic[MUSIC_NUM];
    std::shared_ptr<DirectX::SoundEffectInstance>	mMusicInst[MUSIC_NUM];
    float											mMusicVolume[MUSIC_NUM];

    // 効果音用
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

    void MusicStopAll(); // 全部ストップ
    void MusicPlay(int slot);   // 再生
    void MusicStop(int slot);   // 停止
    void MusicPause(int slot);  // 一時停止
    void MusicResume(int slot); // 再開
    void MusicSetVolume(int slot, float volume); //音量設定

    void SoundPlay(int slot); 

    static Audio& GetInstance()
    {
        static Audio instance;
        return instance;
    }
};

#define AUDIO (Audio::GetInstance())