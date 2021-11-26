#pragma once
#include "MenuBase.h"

#include "lib/Font.h"

#include "MenuCharacterSelect.h"

/*
	メニューのステータス表示画面
*/
class MenuStatus : public MenuBase
{
	// 定数
	static constexpr float BOARD_X = 512.0f;			 // ステータスを表示するボードのX座標
	static constexpr float BOARD_Y = 128.0f;			 // ステータスを表示するボードのY座標
	static constexpr float CHARA_BOARD_OFFSET_X = 10.0f; // キャラクター選択ボードのオフセットX

	// 各種パラメータのフォントのオフセット
	static constexpr float NAME_OFFSET_Y = 10.0f;  
	static constexpr float STATUS_OFFSET_X = 20.0f;
	static constexpr float CUR_OFFSET_X = 96.0f;
	static constexpr float DELIM_OFFSET_X = 4.0f;
	
	static constexpr float STATUS_OFFSET_Y = 60.0f;
	static constexpr float STATUS_ADD_Y = 34.0f;

private: // 変数
	enum StatusKind
	{
		NAME,
		HP,
		MP,
		ATK,
		DEF,
		SPD,
		STATUS_NUM
	};

	std::unique_ptr<Sprite> mStatusPlate; // ステータスのボードの画像
	MenuCharacterSelect mCharacterSelect; // キャラクター選択クラス
	Font mFont; // フォントクラス

public: // 関数

	// コンストラクタ
	MenuStatus() = default;

	// デストラクタ
	~MenuStatus() = default;

	// 初期化
	void Initialize(const PlayerManager* plm) override;
	
	// 更新
	Select Update(PlayerManager* plm) override;
	
	// 描画
	void Render() override;
	
	// 解放
	void Release() override;
};