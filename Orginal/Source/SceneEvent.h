#pragma once
#include "SceneBase.h"

#include <memory>

#include "CameraEvent.h"
#include "EventData.h"

class PlayerManager;
class EnemyManager;
class Terrain;

// �C�x���g�̃V�[��
class SceneEvent : public SceneBase
{
	// �ϐ�
	std::vector<Character> mCharacters; // �o�Ă���L�����N�^�[���X�g
	std::unique_ptr<Terrain> mTerrain;	// �n�`
	std::unique_ptr<CameraEvent> mCamera; // �J����
	PlayerManager* mPMRef = nullptr;	// playermanager�̎Q��(�A�C�e���Ƃ��n���Ƃ��Ɏg��)
	int mEventID; // �C�x���g�ԍ�
	int mFrame = 0; // ���݂̃t���[����(�C�x���g�̐i�s�Ǘ��p)

	struct CharaData
	{
		Vector3 targetPos;
		int motionType;
	};

	struct CameraData
	{
		Vector3 targetPos;
		Vector3 target;
		float lerpFactor;
		CameraEvent::Mode mode;
	};

	struct TextData
	{
		int charaID;
		std::wstring text;
	};

	struct MainData
	{
		int frame = 0;
		std::vector<CharaData> charadata;
		CameraData cameraData;
		std::vector<TextData> textdata;
	};

private: // �֐�

	// �����f�[�^�ǂݍ���(��Ƀ��f���ǂݍ���)
	void LoadEventInitData();

	// ���C���̃f�[�^�ǂݍ���
	bool LoadEventMainData();

public:
	SceneEvent(int eventID, PlayerManager* plm, EnemyManager* em, Terrain* terrain);
	SceneEvent(EventData::EventID eventID);
	~SceneEvent();

	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;
};