#pragma once
#include "IDecideCommand.h"

#include "lib/Font.h"
#include "lib/TVector.h"

class Sprite;

/*
	戦闘時の行動を決定するクラス
	攻撃、スキル、防御etc... 的な
*/
class DecideBehaviour : public IDecideCommand
{
	// 定数
	static const int COMMAND_MIN_X = 0;	   // コマンドXの最低値
	static const int COMMAND_MAX_X = 2;    // コマンドXの最高値
	static const int COMMAND_MIN_Y = 0;    // コマンドYの最低値
	static const int COMMAND_MAX_Y = 2;    // コマンドYの最高値
	static const int COMMAND_NUM = 5;	   // コマンドの数
	static const int COMMAND_VERTICAL = 3; // コマンドの縦の数

	static constexpr float ICON_SIZE = 256.0f;	// アイコンの実サイズ
	static constexpr float ICON_SCALE = 0.25f;  // アイコンのスケール
	static constexpr float ICON_SELECT_X = ICON_SIZE * 5; // アイコンの選択画像のテクスチャ座標(px)
	static constexpr float ICON_BASE_X   = ICON_SIZE * 6; // アイコンの下地のテクスチャ座標(px)

private:// 変数
	TVector2<int> mCommandIndex;			   // 選択中のコマンド
	std::unique_ptr<Sprite> mIcons;			   // アイコンの画像
	std::unique_ptr<Sprite> mCommandNameBoard; // コマンド名を表示するボードの画像
	Font mFont;								   // フォントクラス

public: // 関数
	
	// コンストラクタ
	DecideBehaviour();

	// デストラクタ
	~DecideBehaviour() = default;

	// 初期化
	void Initialize(const BattleCharacterManager* bcm) override;
	
	// 更新
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	
	// 描画
	void Render() override;

	// パラメータリセット
	void ResetParam() override;
};