#include "SceneEvent.h"

#include "Enemy.h"
#include "EnemyManager.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Terrain.h"

#include "CameraEvent.h"
#include "Character.h"
#include "MeshManager.h"
#include "Singleton.h"
#include "XMLLoader.h"

void SceneEvent::LoadEventInitData()
{
	// ファイル取得
	std::string filepath = EventData::GetEventFilePath(mEventID);

	// ファイルオープン
	XMLLoader loader;
	loader.Open(filepath.c_str());

	//initタグの処理
	std::vector<std::string> nodes;
	loader.GetTagNode("init", &nodes);
	for (auto& node : nodes)
	{
		// 属性で分割
		std::vector<std::string> splits;
		loader.NodeSplitElement(node, &splits);

		// 変数準備
		int id = 0;
		Vector3 pos = {};
		Vector3 target = {};
		float scale = 0.0f;
		float angleY = 0.0f;
		// 属性値取得
		std::string element;
		std::string value;
		for (int i = 1; i < splits.size(); ++i)
		{
			// 属性名、属性値を取得
			loader.GetElementValue(splits[i], &element, &value);

			// 分岐
			if (element == "id") id = std::stoi(value);
			else if (element == "pos_x") pos.x = std::stof(value);
			else if (element == "pos_y") pos.y = std::stof(value);
			else if (element == "pos_z") pos.z = std::stof(value);
			else if (element == "scale") scale = std::stof(value);
			else if (element == "angle_y") angleY = std::stof(value);
			else if (element == "target_x") target.x = std::stof(value);
			else if (element == "target_y") target.y = std::stof(value);
			else if (element == "target_z") target.z = std::stof(value);
		}

		if (splits[0] == "character")
		{
			Character chara(id, Character::NONE);
			chara.SetPos(pos);
			chara.SetAngleY(angleY);
			chara.SetScale(Vector3(scale, scale, scale));
			chara.SetMotion(Character::MotionType::IDLE);
			chara.UpdateWorld();
			mCharacters.emplace_back(chara);
		}
		if (splits[0] == "terrain")
		{
			mTerrain = std::make_unique<Terrain>(id);
		}
		if (splits[0] == "camera")
		{
			mCamera->SetPos(pos);
			mCamera->SetTarget(target);
		}
	}
	

	// mainのデータ処理、保存
	loader.GetTagNode("main", &nodes);
	for (auto& node : nodes)
	{
		// 属性で分割
		std::vector<std::string> splits;
		loader.NodeSplitElement(node, &splits);

		// 変数準備
		int id = 0;
		Vector3 pos = {};
		Vector3 target = {};
		float scale = 0.0f;
		float angleY = 0.0f;
		// 属性値取得
		std::string element;
		std::string value;
		for (int i = 1; i < splits.size(); ++i)
		{
			// 属性名、属性値を取得
			loader.GetElementValue(splits[i], &element, &value);

			// 分岐
			if (element == "id") id = std::stoi(value);
			else if (element == "pos_x") pos.x = std::stof(value);
			else if (element == "pos_y") pos.y = std::stof(value);
			else if (element == "pos_z") pos.z = std::stof(value);
			else if (element == "scale") scale = std::stof(value);
			else if (element == "angle_y") angleY = std::stof(value);
			else if (element == "target_x") target.x = std::stof(value);
			else if (element == "target_y") target.y = std::stof(value);
			else if (element == "target_z") target.z = std::stof(value);
		}

		if (splits[0] == "character")
		{
			Character chara(id, Character::NONE);
			chara.SetPos(pos);
			chara.SetAngleY(angleY);
			chara.SetScale(Vector3(scale, scale, scale));
			chara.SetMotion(Character::MotionType::IDLE);
			chara.UpdateWorld();
			mCharacters.emplace_back(chara);
		}
		if (splits[0] == "terrain")
		{
			mTerrain = std::make_unique<Terrain>(id);
		}
		if (splits[0] == "camera")
		{
			mCamera->SetPos(pos);
			mCamera->SetTarget(target);
		}
	}

	loader.Close();
}

SceneEvent::SceneEvent(int eventID, PlayerManager* plm, EnemyManager* em, Terrain* terrain)
{
	//mPlayerManager = std::make_unique<PlayerManager>();
	//mEnemyManager = std::make_unique<EnemyManager>();
	//mTerrain = std::make_unique<Terrain>(terrain->GetID());

	//for (size_t i = 0; i < plm->GetNum(); ++i)
	//{
	//	Player* fieldPl = plm->GetPlayer(i);

	//	mPlayerManager->Create(fieldPl->GetCharaID());
	//	Player* pl = mPlayerManager->GetPlayer(i);
	//	pl->SetPos(fieldPl->GetPos());
	//	pl->SetAngle(pl->GetAngle());
	//}

	//for (size_t i = 0; i < em->GetNum(); ++i)
	//{
	//	mEnemyManager->Create(em->GetEnemy(i)->GetCharaID());
	//}
}

SceneEvent::SceneEvent(EventData::EventID eventID)
{
	// カメラ作成
	mCamera = std::make_unique<CameraEvent>();

	// プレイヤマネージャ保存
	//mPMRef = plm;

	// イベントID保存
	mEventID = (int)eventID;
}

SceneEvent::~SceneEvent()
{
}

void SceneEvent::Initialize()
{
	// イベント初期データ読み込み
	LoadEventInitData();
}

void SceneEvent::Update()
{

}

void SceneEvent::Render()
{
}

void SceneEvent::Release()
{
}
