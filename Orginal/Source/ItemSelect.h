#pragma once
#include "lib/Font.h"
#include "lib/Vector.h"

#include "Item.h"

class Sprite;

/*
	消費アイテムを選ぶクラス
	使いまわせそうだし、1つのクラスに書くと量が増えそうだったからクラス化
*/
class ItemSelect
{
public: // 定数
	static constexpr float ICON_OFFSET = 10.0f; // ボードの左上からアイコン描画位置までの距離(XY両方)

private:

	// アイコン関係
	static const int HORIZONTAL_NUM = 5; // アイコン横列の数
	static const int VERTICAL_NUM = 6;   // アイコン縦列の数
	static constexpr float ICON_SIZE = 256.0f; // アイコンの実サイズ
	static constexpr float ICON_SCALE = 0.25f; // アイコンのスケール
		
	// アイテム情報関係
	static constexpr float INFO_BOARD_OFFSET_X = 512.0f; // アイテム選択画像からのオフセットX
	static constexpr float INFO_ICON_OFFSET_X = 114.0f;  // 情報画像に表示されるアイコンのオフセットX
	static constexpr float INFO_ICON_OFFSET_Y = 76.0f;   // 情報画像に表示されるアイコンのオフセットY
	static constexpr float INFO_ICON_SCALE = 0.5f;		 // 情報画像に表示されるアイコンのスケール
	static constexpr float INFO_TEXT_OFFSET_X = 25;		 // 情報画像に表示されるテキストのオフセットX
	static constexpr float INFO_TEXT_OFFSET_Y = 255;	 // 情報画像に表示されるテキストのオフセットY
	static constexpr float INFO_TEXT_ADD_Y = 45;		 // 2行目以降のテキストのオフセットY
	static constexpr float INFO_WIDTH_MAX = 300;		 // テキストの最大横幅
	static const int INFO_LINE_MAX = 3;					 // テキストの最大行数

private: // 変数
	
	// 画像関係
	std::unique_ptr<Sprite> mItemBoard;	  // アイコンが表示される画像
	std::unique_ptr<Sprite> mSelectFrame; // 選択している場所に表示される画像
	std::unique_ptr<Sprite> mInfoBoard;   // 選択しているアイテムの詳細が表示される画像

	// インベントリ関係
	const ItemInventory* mInventoryRef = nullptr; // 参照しているインベントリ
	int mSelectIndex  = 0;						  // 現在選択している場所

	// 情報関係
	Font mFont;						 // フォント
	std::vector<std::wstring> mInfo; // アイテム情報のテキスト

private: // 関数

	// アイテム情報を更新
	void UpdateInfo();

	// アイテム情報画像に表示されるフォントの設定
	void RenderSetInfo(const Vector2& infoBoardPos);

public:
	
	// コンストラクタ
	ItemSelect() = default;

	// デストラクタ
	~ItemSelect() = default;

	// 初期化関数
	void Initialize();

	// 更新関数
	void Update(const ItemInventory* inventory);

	// 描画関数
	void Render(const Vector2& boardOffset);

	int GetIndex() const { return mSelectIndex; } // 現在選択している場所を取得
	Vector2 GetCursorRightUpPos() const;		  // ボードの位置を含まない(0, 0)基準
};