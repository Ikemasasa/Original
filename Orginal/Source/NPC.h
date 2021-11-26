#pragma once
#include "Character.h"

#include "NPCTextData.h"


/*
	フィールドのNPCキャラクタークラス
	会話したときのどのテキストを参照するかの処理をしている
*/
class NPC : public Character
{
	// 定数
	static const int TALK_COUNT_INIT = 1; // 話しかけ回数の初期値

private: // 変数
	std::vector<NPCTextData::TextData> mTextData; // 表示するテキスト
	int mTalkCount; // 話しかけられ回数
	bool mOldFlag;  // 前回のフラグ(これもボス討伐フラグといっしょに要改善)

public: // 関数

	// コンストラクタ
	NPC(int charaID);
	
	// デストラクタ
	~NPC();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 解放
	void Release();

	// 表示するテキストを取得
	std::vector<std::wstring> GetTexts(); 
	
	// NPCの名前を取得
	std::wstring GetName();				  
};