#pragma once
#include <memory>
#include <stack>

#include "MenuSelect.h"
#include "MenuItem.h"

class Sprite;
class PlayerManager;

// メニュー画面のマネージャ
class MenuManager
{
	// 変数
	std::unique_ptr<Sprite> mBG;					  // 背景画像
	std::stack<std::unique_ptr<MenuBase>> mMenuStack; // メニュー画面のスタック
	MenuBase::Select mNextState;					  //次の画面の識別子


public: // 関数

	// コンストラクタ
	MenuManager();

	// 初期化
	void Initialize(const PlayerManager* plm);
	
	// 更新
	void Update(PlayerManager* plm);
	
	// 描画
	void Render();
	
	// 描画
	void Release();
};