#pragma once
#include <memory>

#include "lib/Font.h"

class PlayerManager;
class Sprite;

/*
	参照するキャラクターを決めるクラス
	誰のインベントリを見るか、とか
	誰のステータスを見るか、とかで使ってる
*/
class MenuCharacterSelect
{
	// 定数
	static constexpr float FONT_OFFSET_X = 2;  // 画像からフォント描画位置までの距離
	static constexpr float FONT_OFFSET_Y = 32; // フォントサイズ / 2

private: // 変数
	std::unique_ptr<Sprite> mBoard;			// 下地の画像
	int mSelectIndex = 0;					// 選択場所
	int mOldSelectIndex = 0;				// 1フレーム前の選択場所
	std::vector<wchar_t> mPlayerNameFirst;	// キャラクターの1文字目のリスト
	Font mFont;							    // フォントクラス

public: // 関数

	// 初期化
	void Initialize(const PlayerManager* plm);

	// 更新
	void Update();

	// 描画
	void Render(Vector2 leftBottom);

	// 解放
	void Release();

	// 1フレーム前から選択場所が変わったか
	bool IsChangedIndex()const { return mSelectIndex != mOldSelectIndex; }

	// 現在の選択場所を取得
	int GetIndex() const { return mSelectIndex; }
};