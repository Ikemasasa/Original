#pragma once
#include "MenuBase.h"

#include <memory>
#include <vector>
#include <string>

#include "lib/Vector.h"
#include "lib/Font.h"

#include "HealthGauge.h"

class Sprite;

// どのメニュー画面に行くかの選択クラス
class MenuSelect : public MenuBase
{
	// 定数
	static constexpr float BOARD_POS_X = 100.0f;	// 各メニュ―画面のテキストのボードのX座標
	static constexpr float BOARD_POS_Y = 75.0f;		// 各メニュ―画面のテキストのボードのY座標
	static constexpr float BOARD_OFFSET_Y = 15.0f;  // 各メニュ―画面のテキストのボードのY座標の感覚
	static const int STR_OFFSET_X = 5;				// 各メニュ―画面のテキストのボードからのX位置
	static const int STR_OFFSET_Y = 5;				// 各メニュ―画面のテキストのボードからのY位置
	static constexpr float MOVE_MAX = 30.0f;		// 開始演出の動く最大距離(data.pos + MOVE_MAX の位置が最初に位置になる)

	static constexpr float HEALTH_BOARD_POS_X = 800.0f; //体力ゲージのボードのX座標
	static constexpr float HEALTH_BOARD_POS_Y = 75.0f; //体力ゲージのボードのY座標
	static constexpr float FADE = 0.15f; // フェードのスピード


private: // 変数
	struct Data
	{
		Vector2 pos;
		float moveX;
	};

	std::unique_ptr<Sprite> mStrBoard;		   // 各メニュ―画面のテキストのボードの画像
	std::unique_ptr<Sprite> mStrSelect;		   // 各メニュ―画面のテキストのボードの選択画像
	std::vector<HealthGauge> mHealthGauge;	   // ヘルスゲージ作成クラス
	std::vector<Data> mDatas;				   //  各メニュ―画面のテキストのボードの座標データ
	Font mFont;								   // フォントクラス

public:// 関数

	// コンストラクタ
	MenuSelect() = default;

	// デストラクタ
	~MenuSelect() = default;

	// 開始演出
	void BeginAnimation();

	// 初期化
	void Initialize(const PlayerManager* plm) override;
	
	// 更新
	Select Update(PlayerManager* plm) override;
	
	// 描画
	void Render() override;
	
	// 解放
	void Release() override;
};