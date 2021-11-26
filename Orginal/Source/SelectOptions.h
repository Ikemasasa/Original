#pragma once
#include <memory>
#include <string>

#include "lib/Font.h"
#include "lib/Vector.h"

class Sprite;

// 選択肢を選ぶクラス
class SelectOptions
{
	// 定数
	static constexpr float TITLE_STR_OFFSET_Y = 5.0f;  // 選択肢のタイトルのオフセットY
	static constexpr float OPTION_STR_OFFSET_X = 7.0f; // 選択肢のオフセットX
	static constexpr float SELECT_OFFSET_X = 5.0f;	   // 選択画像のオフセットX

private: // 変数
	std::unique_ptr<Sprite> mOptionTitle;  // 選択肢のタイトル表示場所の画像
	std::unique_ptr<Sprite> mOption;	   // 選択肢の表示場所の画像
	std::unique_ptr<Sprite> mOptionEnd;	   // 画像の一番下
	std::unique_ptr<Sprite> mOptionSelect; // 選択画像
	
	Font mFont;							   // フォント
	std::wstring mStrTitle;				   // タイトルのテキスト
	std::vector<std::wstring> mStrOptions; // 選択肢のテキスト
	int mOptionIndex = 0;				   // 現在選んでる選択肢

public:// 関数

	// コンストラクタ
	SelectOptions() = default;

	// デストラクタ
	~SelectOptions() = default;

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Render(const Vector2& leftTop, bool isStrsClear = true);

	// 選択肢のタイトルを設定
	void AddTitle(const wchar_t* str);

	// 選択肢を追加
	void AddOption(const wchar_t* str);

	// 選んでる選択肢を取得
	int GetIndex() const { return mOptionIndex; }
};