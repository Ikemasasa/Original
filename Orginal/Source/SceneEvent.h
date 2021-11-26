#pragma once
#include "SceneBase.h"

#include <memory>

#include "CameraEvent.h"
#include "EventData.h"

class PlayerManager;
class EnemyManager;
class Terrain;

// イベントのシーン
class SceneEvent : public SceneBase
{
	// 変数
	std::vector<Character> mCharacters; // 出てくるキャラクターリスト
	std::unique_ptr<Terrain> mTerrain;	// 地形
	std::unique_ptr<CameraEvent> mCamera; // カメラ
	PlayerManager* mPMRef = nullptr;	// playermanagerの参照(アイテムとか渡すときに使う)
	int mEventID; // イベント番号
	int mFrame = 0; // 現在のフレーム数(イベントの進行管理用)

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

private: // 関数

	// 初期データ読み込み(主にモデル読み込み)
	void LoadEventInitData();

	// メインのデータ読み込み
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