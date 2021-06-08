#include "Audio.h"
#include "DirectXTK/Inc/SoundCommon.h"
#include "DirectXTK/Inc/WAVFileReader.h"

using namespace DirectX;

Audio::Audio()
{
	// Audioオブジェクトの生成
	AUDIO_ENGINE_FLAGS eFlags = AUDIO_ENGINE_FLAGS::AudioEngine_Default;
#ifdef _DEBUG
	eFlags = eFlags | AudioEngine_Debug;
#endif
	mAudioEngine = std::make_unique<AudioEngine>(eFlags);

	// ボリュームの初期化
	for (auto& v : mMusicVolume) v = 1.0f;
	for (auto& v : mSoundVolume) v = 1.0f;
}

Audio::~Audio()
{
	for (int i = 0; i < MUSIC_NUM; ++i) UnLoadMusic(i);
	for (int i = 0; i < SOUND_NUM; ++i) UnLoadSound(i);

	mAudioEngine.reset();
}

void Audio::Initialize()
{
	// TODO : 仮
	LoadMusic((int)Music::TITLE, L"Data/Audio/Music/title.wav");
	LoadMusic((int)Music::FIELD_REMAINS, L"Data/Audio/Music/title.wav");

	LoadSound((int)Sound::SELECT, L"Data/Audio/Sound/select.wav");
	LoadSound((int)Sound::SELECT_LONG, L"Data/Audio/Sound/select_long.wav");
	LoadSound((int)Sound::CANCEL, L"Data/Audio/Sound/cancel.wav");
	LoadSound((int)Sound::CURSOR_MOVE, L"Data/Audio/Sound/cursor_move.wav");
	LoadSound((int)Sound::HEAL, L"Data/Audio/Sound/heal.wav");
}

void Audio::LoadMusic(int slot, const wchar_t* waveFilename, float volume)
{
	if (slot >= MUSIC_NUM) return;
	if (mMusicInst[slot]) mMusicInst[slot].reset();
	mMusic[slot].reset(new SoundEffect(mAudioEngine.get(), waveFilename));
	mMusicVolume[slot] = volume;
}

void Audio::UnLoadMusic(int slot)
{
	if (slot >= MUSIC_NUM) return;
	if(mMusicInst[slot]) mMusicInst[slot].reset();
	mMusic[slot].reset();
}

void Audio::LoadSound(int slot, const wchar_t* waveFilename, float volume)
{
	if (slot >= SOUND_NUM) return;
	if (mSoundInst[slot]) mSoundInst[slot].reset();
	mSound[slot].reset(new SoundEffect(mAudioEngine.get(), waveFilename));
	mSoundVolume[slot] = volume;
}

void Audio::UnLoadSound(int slot)
{
	if (slot >= SOUND_NUM) return;
	if (mSoundInst[slot]) mSoundInst[slot].reset();
	mSound[slot].reset();
}

void Audio::MusicPlay(int slot)
{
	if (!mMusic[slot]) return;
	mMusicInst[slot] = mMusic[slot]->CreateInstance();
	mMusicInst[slot]->Play(true); // ループする
	mMusicInst[slot]->SetVolume(mMusicVolume[slot]);
}

void Audio::MusicStop(int slot)
{
	if (!mMusicInst[slot]) return;
	mMusicInst[slot]->Stop();
}

void Audio::MusicPause(int slot)
{
	if (!mMusicInst[slot]) return;
	mMusicInst[slot]->Pause();
}

void Audio::MusicResume(int slot)
{
	if (!mMusicInst[slot]) return;
	mMusicInst[slot]->Resume();
}

void Audio::MusicSetVolume(int slot, float volume)
{
	if (!mMusicInst[slot]) return;
	mMusicInst[slot]->SetVolume(volume);
}

void Audio::SoundPlay(int slot)
{
	if (!mSound[slot]) return;
	if (mSoundInst[slot])
	{
		mSoundInst[slot]->Stop();
	}
	else
	{
		mSoundInst[slot] = mSound[slot]->CreateInstance();
	}

	mSoundInst[slot]->Play(false); // ループしない
	mSoundInst[slot]->SetVolume(mMusicVolume[slot]);
}
