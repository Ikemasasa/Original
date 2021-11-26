#pragma once
#include <memory>
#include <string>
#include <list>

#include "lib/Font.h"
#include "lib/Sprite.h"

#include "Define.h"

/*
	キーの動作を表示するクラス
*/
class KeyGuide final
{
	// 実態作成禁止
	KeyGuide();
	~KeyGuide() = default;

public:
	enum Key
	{
		A,
		B,
		X,
		Y,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DPAD,
		LB,
		RB,
	};

private: // 定数

	// アイコン関係
	static constexpr float ICON_SIZE = 128.0f;						 // キーのアイコンの実サイズ
	static constexpr float ICON_SCALE = 0.25;						 // キーのアイコンのスケール
	static constexpr float ICON_SCALE_SIZE = ICON_SIZE * ICON_SCALE; // キーのアイコンのスケールを反映させたサイズ
	static constexpr float GUIDE_LEFTTOP_X = 12.0f;					 // 一番左のアイコンのオフセットX
	static constexpr float GUIDE_LEFTTOP_Y = 5.0f;					 // 一番左のアイコンのオフセットY
	static constexpr float BOARD_LEFTTOP_Y = Define::SCREEN_HEIGHT - ICON_SCALE_SIZE * 2.0f; // キーのアイコンが表示される画像のY座標

	// 線形補完の係数
	static constexpr float LERP_FACTOR = 0.1f;

	struct Guide
	{
		std::vector<Key> keys;
		std::wstring description;
	};

private: // 変数

	// 画像
	std::unique_ptr<Sprite> mKeyIcons;		// キーのアイコン
	std::unique_ptr<Sprite> mKeyGuideBoard; // キーのアイコンが表示される画像
	
	// キーガイド関係
	std::list<Guide> mKeyGuides;		 // 表示するキーガイドのリスト
	Font mFont;							 // フォント
	float mMoveX = Define::SCREEN_WIDTH; // 左側に動く量


private: // 関数

	// 上のEnum順に表示するために挿入場所を取得する関数
	std::list<Guide>::iterator GetInsertIterator(Key key);

	// キーがすでにあるかチェック
	bool ExistKey(Key key);

public:

	// キーガイド追加
	void Add(Key key, std::wstring description);
	void Add(Key* key, int keyNum, std::wstring description);

	// 描画
	void Render(bool isClear);


	void ResetMoveX() { mMoveX = Define::SCREEN_WIDTH; };


	// シングルトン
	static KeyGuide& Instance()
	{
		static KeyGuide inst;
		return inst;
	}
};