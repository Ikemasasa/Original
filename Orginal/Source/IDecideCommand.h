#pragma once
#include <memory>

// 前方宣言
class BattleCharacter;
class BattleCharacterManager;
class CommandBase;

/*
	戦闘時のコマンド選択の基底クラス
*/
class IDecideCommand
{
public:
	enum class CmdState
	{
		ITEM,
		SELECT_PARTY,
		SELECT_ENEMY,

		MAX,
		BACK,
		NONE,
	};

protected: // 変数
	std::unique_ptr<IDecideCommand> mNextCommand; // 次のコマンド
	int mSelectIndex = 0;						  // 選択中の場所
	bool mIsBackState = false;					  // コマンドを1つ戻すか

public: // 関数

	// コンストラクタ
	IDecideCommand() = default;

	// デストラクタ
	virtual ~IDecideCommand() = default;

	// 初期化(純粋仮想)
	virtual void Initialize(const BattleCharacterManager* bcm) = 0;

	// 更新(純粋仮想)
	virtual void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) = 0;
	
	// 描画
	virtual void Render() {}

	// 決定とキャンセルの効果音再生(ほとんどの場所で書くから関数化)
	void SoundPlay(); 

	// パラメータリセット
	virtual void ResetParam() {};

	// コマンドを1つ戻すか取得
	bool IsBackState() const { return mIsBackState; }

	// 現在選択中の場所を取得
	int GetSelectIndex() const { return mSelectIndex; }

	// 次のコマンドのポインタを取得
	IDecideCommand* GetNextCommandState() { return mNextCommand.release(); } 
};