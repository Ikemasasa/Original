#include "Audio.h"

#include "DirectXTK/Inc/SoundCommon.h"
#include "DirectXTK/Inc/WAVFileReader.h"

#include "Math.h"

using namespace DirectX;

// static �����o�ϐ�
std::unique_ptr<DirectX::AudioEngine>			Audio::mAudioEngine;
std::unique_ptr<DirectX::SoundEffect>			Audio::mMusic[MUSIC_NUM];
std::unique_ptr<DirectX::SoundEffectInstance>	Audio::mMusicInst[MUSIC_INST_NUM];
float											Audio::mMusicVolume[MUSIC_INST_NUM];
std::unique_ptr<DirectX::SoundEffect>			Audio::mSound[SOUND_NUM];
std::unique_ptr<DirectX::SoundEffectInstance>	Audio::mSoundInst[SOUND_INST_NUM];
float											Audio::mSoundVolume[SOUND_INST_NUM];

void Audio::Initialize()
{
	// Audio�I�u�W�F�N�g�̐���
	AUDIO_ENGINE_FLAGS eFlags = AUDIO_ENGINE_FLAGS::AudioEngine_Default;
#ifdef _DEBUG
	eFlags = eFlags | AudioEngine_Debug;
#endif
	mAudioEngine = std::make_unique<AudioEngine>(eFlags);

	// �{�����[���̏�����
	for (auto& v : mMusicVolume) v = 0.5f;
	for (auto& v : mSoundVolume) v = 1.0f;
}

void Audio::Update()
{
	MusicFade();
}

void Audio::Release()
{
	UnLoadAll();

	mAudioEngine.reset();
}

void Audio::LoadMusic(int slot, const wchar_t* waveFilename, float volume)
{
	if (slot >= MUSIC_NUM) return;
	if (mMusicInst[slot]) mMusicInst[slot].reset();
	mMusic[slot].reset(new SoundEffect(mAudioEngine.get(), waveFilename));
	mMusicVolume[slot] = volume;
}

void Audio::LoadSound(int slot, const wchar_t* waveFilename, float volume)
{
	if (slot >= SOUND_NUM) return;
	if (mSoundInst[slot]) mSoundInst[slot].reset();
	mSound[slot].reset(new SoundEffect(mAudioEngine.get(), waveFilename));
	mSoundVolume[slot] = volume;
}

void Audio::MusicStopAll()
{
	for (int i = 0; i < MUSIC_INST_NUM; ++i)
	{
		if (!mMusicInst[i]) return;
		mMusicInst[i]->Stop(false);
	}
}

void Audio::MusicPlay(int slot)
{
	if (!mMusic[slot]) return;

	int instSlot = -1;
	// ���C���X���b�g���g���ĂȂ��Ȃ�
	if (!mMusicInst[MUSIC_INST_MAIN_SLOT])
	{
		// ���C���X���b�g�ɂ��̂܂܂����
		instSlot = MUSIC_INST_MAIN_SLOT;
	}
	else // ���C���X���b�g���g�p���Ȃ�
	{
		// �T�u�X���b�g�ɓ����
		instSlot = MUSIC_INST_SUB_SLOT;
	}

	// �C���X�^���X����
	mMusicInst[instSlot] = mMusic[slot]->CreateInstance();
	mMusicVolume[instSlot] = 0.0f;
	mMusicInst[instSlot]->SetVolume(0.0f); // Update�Ńt�F�[�h�C������
	mMusicInst[instSlot]->Play(true); // ���[�v����
}

void Audio::MusicPause()
{
	if (mMusicInst[MUSIC_INST_MAIN_SLOT]) mMusicInst[MUSIC_INST_MAIN_SLOT]->Pause();
	if (mMusicInst[MUSIC_INST_SUB_SLOT])  mMusicInst[MUSIC_INST_SUB_SLOT]->Pause();
}

void Audio::MusicResume()
{
	if (mMusicInst[MUSIC_INST_MAIN_SLOT]) mMusicInst[MUSIC_INST_MAIN_SLOT]->Resume();
	if (mMusicInst[MUSIC_INST_SUB_SLOT])  mMusicInst[MUSIC_INST_SUB_SLOT]->Resume();
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

	mSoundInst[slot]->Play(false); // ���[�v���Ȃ�
	mSoundInst[slot]->SetVolume(mMusicVolume[slot]);
}

void Audio::SoundStop(int slot)
{
	if (!mSoundInst[slot]) return;
	mSoundInst[slot]->Stop();
}

void Audio::MusicFade()
{
	// music�̃t�F�[�h�̍X�V
	const float FADE = MUSIC_VOLUME_MAX / FADE_FRAME;

	// ���C���X���b�g�g�p��
	if (mMusicInst[MUSIC_INST_MAIN_SLOT])
	{
		// �T�u�X���b�g���g�p���Ȃ� 
		if (mMusicInst[MUSIC_INST_SUB_SLOT])
		{
			// ���C���X���b�g�t�F�[�h�A�E�g
			mMusicVolume[MUSIC_INST_MAIN_SLOT] = Math::Max(mMusicVolume[MUSIC_INST_MAIN_SLOT] - FADE, 0.0f);
			mMusicInst[MUSIC_INST_MAIN_SLOT]->SetVolume(mMusicVolume[MUSIC_INST_MAIN_SLOT]);

			// �T�u�X���b�g�t�F�[�h�C��
			mMusicVolume[MUSIC_INST_SUB_SLOT] = Math::Min(mMusicVolume[MUSIC_INST_SUB_SLOT] + FADE, MUSIC_VOLUME_MAX);
			mMusicInst[MUSIC_INST_SUB_SLOT]->SetVolume(mMusicVolume[MUSIC_INST_SUB_SLOT]);

			// ���C���X���b�g�̃t�F�[�h�A�E�g���I�������
			if (mMusicVolume[MUSIC_INST_MAIN_SLOT] <= 0.0f)
			{
				// ���C���X���b�g�ɃT�u�X���b�g������
				mMusicInst[MUSIC_INST_MAIN_SLOT].reset(mMusicInst[MUSIC_INST_SUB_SLOT].release());
				mMusicVolume[MUSIC_INST_MAIN_SLOT] = mMusicVolume[MUSIC_INST_SUB_SLOT];
				mMusicVolume[MUSIC_INST_SUB_SLOT] = 0.0f;
			}
		}
		else
		{
			// ���C���X���b�g�t�F�[�h�C��
			mMusicVolume[MUSIC_INST_MAIN_SLOT] = Math::Min(mMusicVolume[MUSIC_INST_MAIN_SLOT] + FADE, MUSIC_VOLUME_MAX);
			mMusicInst[MUSIC_INST_MAIN_SLOT]->SetVolume(mMusicVolume[MUSIC_INST_MAIN_SLOT]);
		}
	}
}

void Audio::UnLoadAll()
{
	// �C���X�^���X���
	for (int i = 0; i < MUSIC_INST_NUM; ++i) mMusicInst[i].reset();
	for (int i = 0; i < SOUND_INST_NUM; ++i) mSoundInst[i].reset();

	// ���f�[�^���
	for (int i = 0; i < MUSIC_NUM; ++i) mMusic[i].reset();
	for (int i = 0; i < SOUND_NUM; ++i) mSound[i].reset();
}
