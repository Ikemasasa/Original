#pragma once

#include "lib/Audio.h"
#include "lib/Input.h"

class PlayerManager;

/*
	メニュー画面の基底クラス
	各メニュー画面のクラスに継承する
*/
class MenuBase
{
	// フレンドクラス
	friend class MenuManager;

protected: // 変数
	enum Select
	{
		ITEM,
		EQUIPMENTS,
		STATUS,
		SELECT_NUM,

		BACK,
		NONE = 10 // アイテム(ステータスでもいいけど)を一番上にするためにNONEを一番最後にしてる
	};

	int mSelectIndex = 0; // 選択場所

public: // 関数

	// コンストラクタ
	MenuBase() = default;

	// デストラクタ
	virtual ~MenuBase() = default;

	// 初期化
	virtual void Initialize(const PlayerManager* plm) = 0;

	// 更新
	virtual Select Update(PlayerManager* plm) = 0;
	
	// 描画
	virtual void Render() = 0;
	
	// 解放
	virtual void Release() = 0;
};