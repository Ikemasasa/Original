#pragma once
#include <memory>
#include <vector>

#include "lib/Font.h"

#include "SkillData.h"

class Sprite;

/*
	スキル選択クラス
*/
class SkillSelect
{
	// 定数
	static constexpr float SKILLNAME_POS_X = 18.0f;	 // スキル名のテキストのX座標
	static constexpr float SKILLNAME_POS_Y = 18.0f;  // スキル名のテキストのY座標
	static constexpr float USE_MP_OFFSET_X = 280.0f; // MP使用量のテキストのX座標
	static const int VERTICAL_SKILL_MAX = 6;		 // 縦に並べられるスキルの数
	static const int FONT_SIZE = 32;				 // フォントサイズ

	static constexpr float SKILLINFO_POS_X = 14.0f;	 // スキルの説明文のテキストのX座標
	static constexpr float SKILLINFO_POS_Y = 229.0f; // スキルの説明文のテキストのY座標
	static constexpr float INFO_LINE_PX = 420.0f;	 // スキルの説明文のテキストの横幅の最大px

private: // 変数
	std::unique_ptr<Sprite> mSkillBoard;				// スキル一覧のボードの画像
	std::unique_ptr<Sprite> mSkillSelect;				// スキル選択の画像
	std::vector <const SkillData::BaseData*> mSkills;   // 表示するスキルのリスト
	int mSkillIndex = 0;								// 選択場所
	Vector2 mBoardPos;								    // ボードの座標
	Font mFont;											// フォントクラス

public: // 関数

	// 初期化
	void Initialize(const int charaID, const Vector2& leftTop);

	// 更新
	void Update();

	// 描画
	void Render();

	// 選んだスキルのパラメータを取得
	const SkillData::BaseData* GetSelectSkill() const { return mSkills[mSkillIndex]; }

	// 現在選んでる選択場所を取得
	int GetIndex() const { return mSkillIndex; }

	// 使用可能なスキルがあるか
	bool ExistSkill() const { return !mSkills.empty(); }
};