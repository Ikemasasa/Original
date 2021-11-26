#pragma once
#include "MenuBase.h"

#include <vector>

#include "HealthGauge.h"
#include "ItemSelect.h"
#include "MenuCharacterSelect.h"
#include "SelectOptions.h"

class ItemInventory;
class PlayerManager;

/*
	メニューのアイテム使用画面
*/
class MenuItem : public MenuBase
{
	// 定数
	static constexpr float BOARD_OFFSET_X = 300.0f; // アイテム選択ボードのX座標
	static constexpr float BOARD_OFFSET_Y = 150.0f; // アイテム選択ボードのY座標

	static constexpr float OPTION_BOARD_X = 400.0f; // 選択肢のボードのX座標
	static constexpr float OPTION_BOARD_Y = 250.0f; // 選択肢のボードのY座標

	static constexpr float HEALTH_BOARD_X = 900.0f; // キャラクターのヘルス表示ボードのX座標
	static constexpr float HEALTH_BOARD_Y = 100.0f; // キャラクターのヘルス表示ボードのY座標

private:// 変数
	ItemInventory* mInventory; // アイテムインベントリの参照

	int mItemIndex = 0;					   // 選択場所
	bool mIsUseFieldItem = false;		   // フィールド使用アイテムを使うか
	MenuCharacterSelect mCharacterSelect;  // 参照先のキャラクター切り替えクラス
	std::vector<HealthGauge> mHealthGauge; // キャラクターのヘルス表示クラス
	ItemSelect mItemSelect;				   // アイテム選択クラス
	SelectOptions mSelectOptions;		   // 選択肢選択クラス

private: // 関数

	// 回復アイテムを使用
	void UseHealItem(const PlayerManager* plm, const int itemID);
	
	// フィールド使用アイテムを使用
	void UseFieldUseItem(const int itemID);

public:

	// コンストラクタ
	MenuItem() = default;

	// デストラクタ
	~MenuItem() = default;
	
	// 初期化
	void Initialize(const PlayerManager* plm) override;
	
	// 更新
	Select Update(PlayerManager* plm) override;
	
	// 描画
	void Render() override;
	
	// 解放
	void Release() override;
};