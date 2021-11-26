#pragma once
#include <memory>

#include "lib/Font.h"
#include "lib/Sprite.h"

#include "StatusData.h"

/*
	キャラクターのHP,MPをゲージ化するクラス
	中身は文字や画像の位置調整で、はちゃめちゃなので見ないでください
*/
class HealthGauge
{
public: // 変数

	// どこを中心とするか
	enum Center
	{
		LEFTTOP,
		MIDTOP,
		RIGHTTOP,
		LEFTMID,
		MID,
		RIGHTMID,
		LEFTBOTTOM,
		MIDBOTTOM,
		RIGHTBOTTOM
	};

private:
	enum BUFF
	{
		ATK_UP,
		ATK_DOWN,
		DEF_UP,
		DEF_DOWN,

		BUFF_NUM
	};

private: // 定数
	static const int FONT_SIZE = 22;							// フォントサイズ
	static const int FONT_WEIGHT = 22;							// フォントウェイト
	static constexpr float SCALE_X = 0.45f;						// 画像のスケールX
	static constexpr float SCALE_Y = 0.30f;						// 画像のスケールY
	static constexpr float GAUGE_OFFSET = 4 * SCALE_X;			// ゲージ画像のオフセットX
	static constexpr float GAUGE_FIRST_OFFSET_Y = 55 * SCALE_Y; // ゲージ画像のオフセットY
	static constexpr float HP_STR_OFFSET_X = 15 * SCALE_X;		// HPのフォントのオフセットX
	static constexpr float SELECT_GAUGE_ALPHA = 0.6f;		    // 選択画像のアルファ値

	static constexpr float BUFF_ICON_SCALE = 0.15f;								    // バフアイコンのスケール
	static constexpr float BUFF_ICON_SIZE = 128;								    // バフアイコンの実サイズ
	static constexpr float BUFF_ICON_SCALE_SIZE = BUFF_ICON_SIZE * BUFF_ICON_SCALE; // バフアイコンのスケールを反映させたサイズ

private: // 変数
	
	// 画像関係
	std::unique_ptr<Sprite> mBaseBoard;   // 全体の画像
	std::unique_ptr<Sprite> mGauge;		  // ゲージの画像 
	std::unique_ptr<Sprite> mGaugeBoard;  // ゲージが削れた時に出る裏側の画像
	std::unique_ptr<Sprite> mGaugeSelcet; // ゲージ選択画像
	std::unique_ptr<Sprite> mBuffIcons;   // バフのアイコン画像
	Vector2 mCenter;   // 画像の中心
	Vector2 mPos;	   // 画像の座標
	float mRGB = 0.0f; // 画像のRGB割合

	// ゲージ関係
	Font mFont;							 // フォント
	bool mIsInitialize = false;			 // 初期化したかのフラグ
	bool mBuffFlags[BUFF_NUM];			 // どのバフがかかってるかのフラグ
	std::wstring mName;	// キャラの名前
	int mCurrentHP;		// キャラのHP
	float mHPRate;// キャラのHP割合
	int mCurrentMP;// キャラのMP
	float mMPRate;	// キャラのMP割合

private: // 関数
	
	// ステータスの変数に設定する
	void SetStatusValue(const Status& status);

	// 画像の中心を座標に変換して設定する
	void DecideCenter(const Center& center);

	// フォントの描画準備
	void RenderSetFont();

public:
	
	// 初期化関数
	void Initialize();

	// ステータスを設定
	void Set(const Status& status, const Vector2& pos, Center center, float rgb = 1.0f);
	void Set(const Status& status, const Vector3& pos, Center center, float rgb = 1.0f);

	// 描画
	//void Render();
	void Render(bool isSelectRender = false);

	// 選択画像描画
	void RenderSelect();

	float GetWidth() const { return mBaseBoard->GetSize().x * SCALE_X; }  // 画像の横幅を取得
	float GetHeight() const { return mBaseBoard->GetSize().y * SCALE_Y; } // 画像の縦幅を取得
};