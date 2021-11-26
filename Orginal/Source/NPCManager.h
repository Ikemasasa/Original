#pragma once
#include <DirectXMath.h>
#include <memory>
#include <vector>

#include "lib/Vector.h"

#include "NPC.h"
#include "NPCTalk.h"

class Shader;
class Sprite;

/*
	NPCのマネージャクラス
	NPCの生成、更新、描画、会話中のNPCの保存とかをしている
*/
class NPCManager
{
public: // 定数
	static constexpr float TALK_DIST = 8.0f; // 会話可能な最大距離
	static constexpr float TALK_DOT = 0.5f;	 // 会話可能な最低内積値

private:
	static constexpr float ICON_RENDER_DIST = TALK_DIST * 2.0f; // 会話可能のアイコンを表示する最大距離
	static constexpr float ICON_SCALE = 0.125f;					// アイコンのスケール

private: // 変数
	std::vector<std::unique_ptr<NPC>> mNPCs; // NPCのリスト
	std::unique_ptr<Sprite> mTalkIcon;		 // 会話可能アイコン
	Vector3 mPlayerPos = {};				 // プレイヤーの座標

public: // 関数

	// コンストラクタ
	NPCManager() = default;

	// デストラクタ
	~NPCManager() = default;

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Render(const Shader* shader, const Matrix& view, const Matrix& projection, const Vector4& lightDir);
	
	// UIを描画
	void RenderUI();

	// 解放
	void Release();

	// NPCを生成
	void Create(const int charaID);

	// NPCの数を取得
	size_t GetNum() const { return mNPCs.size(); }

	// 引数の番号のNPCを取得
	NPC* GetNPC(size_t index) const { return mNPCs[index].get(); }

	// プレイヤーの座標を設定
	void SetPlayerPos(const Vector3& pos) { mPlayerPos = pos; }
};