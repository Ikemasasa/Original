#pragma once
#include "ItemData.h"
#include "SkillData.h"

class BattleCharacterManager;

// 戦闘時のコマンドを決める基底クラス
class CommandBase
{
public: // 変数
	enum class Behaviour
	{
		NONE,
		USE_ITEM,
		GUARD,
		ATTACK,
		SKILL,
		ESCAPE,

		MAX
	};

protected:
	Behaviour mBehaviour = Behaviour::NONE;	// 決まった行動
	std::vector<int> mTargetObjIDs;			// 対象のObjIDリスト

	bool mIsUseItem = false;		  // アイテムを使うか
	UseItemData::Param mUseItemParam; // アイテムのパラメータ

	bool mIsUseSkill = false;			// スキルを使うか
	SkillData::BaseData mUseSkillParam; // スキルのパラメータ
										
public:// 関数

	// コンストラクタ
	CommandBase() = default;

	// デストラクタ
	virtual ~CommandBase() = default;

	// 更新
	virtual void Update(const BattleCharacterManager* bcm) = 0;

	// 描画
	virtual void Render() const {};
	
	// 行動終了
	virtual void BehaviourFinished();


	// ゲッター
	bool IsUseItem() const { return mIsUseItem; }							   // アイテムを使うかを取得
	bool IsUseSkill() const { return mIsUseSkill; }							   // スキルを使うかを取得
	bool IsBehaviourEnable() const { return (mBehaviour != Behaviour::NONE); } // 行動が決まったかを取得

	std::vector<int> GetTargetObjIDs() const { return mTargetObjIDs; }			 // 対象のObjIDリストを取得
	Behaviour GetBehaviour() const { return mBehaviour; }						 // 行動を取得
	const UseItemData::Param* GetItemParam() const { return &mUseItemParam; }    // アイテムのパラメータ取得
	const SkillData::BaseData* GetSkillParam() const { return &mUseSkillParam; } // スキルのパラメータ取得
	
	void SetBehaviour(const Behaviour behaviour) { mBehaviour = behaviour; }			 // 行動を設定
	void SetItemParam(const UseItemData::Param* param);									 // アイテムのパラメータを設定
	void SetSkillParam(const SkillData::BaseData* param);								 // スキルのパラメータを設定
	void AddTargetObjID(const int targetObjID) { mTargetObjIDs.push_back(targetObjID); } // 対象のObjIDリストに追加
};
