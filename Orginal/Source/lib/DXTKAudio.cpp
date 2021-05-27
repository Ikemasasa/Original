#include "DXTKAudio.h"
#include "DirectXTK/Inc/SoundCommon.h"
#include "DirectXTK/Inc/WAVFileReader.h"

using namespace DirectX;

DXTKAudio::DXTKAudio()
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

DXTKAudio::~DXTKAudio()
{
	mAudioEngine.reset();
}

void DXTKAudio::LoadMusic(int slot, const wchar_t* waveFilename, float volume)
{
	if (slot >= MUSIC_NUM) return;
	if (mMusicInst[slot]) mMusicInst[slot].reset();
	mMusic[slot].reset(new SoundEffect(mAudioEngine.get(), waveFilename));
	mMusicVolume[slot] = volume;
}

void DXTKAudio::UnLoadMusic(int slot)
{
	if (slot >= MUSIC_NUM) return;
	if(mMusicInst[slot]) mMusicInst[slot].reset();
	mMusic[slot].reset();
}

void DXTKAudio::LoadSound(int slot, const wchar_t* waveFilename, float volume)
{
	if (slot >= SOUND_NUM) return;
	if (mSoundInst[slot]) mSoundInst[slot].reset();
	mSound[slot].reset(new SoundEffect(mAudioEngine.get(), waveFilename));
	mSoundVolume[slot] = volume;
}

void DXTKAudio::UnLoadSound(int slot)
{
	if (slot >= SOUND_NUM) return;
	if (mSoundInst[slot]) mSoundInst[slot].reset();
	mSound[slot].reset();
}

void DXTKAudio::MusicPlay(int slot)
{
	if (!mMusic[slot]) return;
	mMusicInst[slot] = mMusic[slot]->CreateInstance();
	mMusicInst[slot]->Play(true); // ループする
	mMusicInst[slot]->SetVolume(mMusicVolume[slot]);
}

void DXTKAudio::MusicStop(int slot)
{
	if (!mMusicInst[slot]) return;
	mMusicInst[slot]->Stop();
}

void DXTKAudio::MusicPause(int slot)
{
	if (!mMusicInst[slot]) return;
	mMusicInst[slot]->Pause();
}

void DXTKAudio::MusicResume(int slot)
{
	if (!mMusicInst[slot]) return;
	mMusicInst[slot]->Resume();
}

void DXTKAudio::MusicSetVolume(int slot, float volume)
{
	if (!mMusicInst[slot]) return;
	mMusicInst[slot]->SetVolume(volume);
}

void DXTKAudio::SoundPlay(int slot)
{
	if (!mMusic[slot]) return;
	mMusicInst[slot] = mMusic[slot]->CreateInstance();
	mMusicInst[slot]->Play(false); // ループしない
	mMusicInst[slot]->SetVolume(mMusicVolume[slot]);
}
