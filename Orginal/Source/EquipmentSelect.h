#pragma once
#include <memory>
#include <vector>

#include "lib/Font.h"

#include "EquipmentInventory.h"

class Sprite;


// 装備選択クラス
class EquipmentSelect
{
	// 装備
	static const int HORIZONTAL_NUM = 5; // アイコン横列の数
	static const int STATUS_NUM = 6;     // アイコン縦列の数
	static const int PARAM_NUM = 3;		 // 装備のパラメータの数

	static constexpr float ICON_OFFSET = 10.0f; // アイコン
	static constexpr float ICON_SIZE = 256.0f;	// アイコンの実サイズ
	static constexpr float ICON_SCALE = 0.25f;  // アイコンのスケール

	static constexpr float INFO_BOARD_OFFSET_X = 400.0f; // ベースボードから詳細のボードまでのオフセットX
	static constexpr float INFO_ICON_OFFSET_X = 114.0f;  // 詳細のボードからアイコン表示場所オフセットX
	static constexpr float INFO_ICON_OFFSET_Y = 76.0f;   // 詳細のボードからアイコン表示場所オフセットY
	static constexpr float INFO_ICON_SCALE = 0.5f;		 // 詳細ボードのアイコンのスケール

private: // 変数
	std::unique_ptr<Sprite> mBoard;			// ベースのボード画像
	std::unique_ptr<Sprite> mSelectFrame;	// 選択した場所に表示する画像
	std::unique_ptr<Sprite> mInfoBoard;		// 詳細のボード
	std::unique_ptr<Sprite> mEquipmentMark; // 装備中のマーク画像
	int mSelectIndex = 0;					// 選択中の場所
	std::vector<EquipmentInventory::Data> mEquipmentsData; // タイプ別の装備品リスト

	Font mFont;						 // フォントクラス
	std::vector<std::wstring> mInfo; // 装備品の説明のテキスト

private: // 関数

	 // 装備品の説明のテキストを設定
	void RenderSetInfo(const Vector2& infoBoardPos);

public:

	// 初期化
	void Initialize();

	// 更新
	int Update(const std::vector<EquipmentInventory::Data>& equipmentsData);

	// 描画
	void Render(const Vector2& boardPos);

	// 装備品リスト
	void ClearData();
};