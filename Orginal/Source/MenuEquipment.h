#pragma once
#include "MenuBase.h"

#include "lib/Font.h"

#include "EquipmentSelect.h"
#include "MenuCharacterSelect.h"

class Sprite;

/*
	メニューの装備品画面
	主に装備したり装備を外したり
*/
class MenuEquipment : public MenuBase
{
	// 定数
	static constexpr float FONT_OFFSET_X = 10.0f; // 装備品タイプのテキストのオフセットX
	static constexpr float FONT_OFFSET_Y = 10.0f; // 装備品タイプのテキストのオフセットY
	static constexpr float FONT_ADD_Y    = 32.0f; // 2行目以降のY座標に加算する値

	static constexpr float BOARD_POS_X = 150.0f; // 装備品のタイプを表示するボードのX座標
	static constexpr float BOARD_POS_Y = 150.0f;// 装備品のタイプを表示するボードのY座標

private: // 変数
	std::unique_ptr<Sprite> mTypeBoard;	 // 装備品タイプを表示するボードの画像
	std::unique_ptr<Sprite> mTypeSelect; // 装備品タイプを選択する画像
	MenuCharacterSelect mCharacterSelect; // キャラクター選択クラス
	EquipmentSelect mEquipmentSelect;	  // 装備品選択クラス
	Font mFont; // フォントクラス
	bool mIsDecideType = false; // タイプが決まったか
	int mOldSelectIndex = 0; // 前回の選択場所

public:// 関数
	
	// 初期化
	void Initialize(const PlayerManager* plm) override;
	
	// 更新
	Select Update(PlayerManager* plm) override;
	
	// 描画
	void Render() override;

	// 解放
	void Release() override;
};