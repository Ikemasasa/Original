#pragma once
#include "IDecideCommand.h"

#include "Character.h"
#include "HealthGauge.h"

class BattleCharacter;
class BattleCharacterManager;
class CommandBase;
class Sprite;

/*
	戦闘時のコマンドで対象を選択するクラス

	誰に何をするっていうのを最終的にここで決めるので、
	一番最後に通るクラス
*/
class DecideTargetChara : public IDecideCommand
{
	// 定数
	static constexpr float ARROW_SCALE = 0.5f;	// 矢印のスケール

private: // 変数
	enum Range {SINGLE, ALL};
	Range mRange = SINGLE; // 対象の数

	std::unique_ptr<Sprite> mArrow; // 矢印の画像
	Character::Type mCharaType;					 // 対象のタイプ
	std::vector<HealthGauge> mHealthGauges;		 // 対象のステータスゲージ
	std::vector<BattleCharacter*> mTargetCharas; // 対象のキャラリスト
	std::vector<int> mEffectHandles;			 // 選択エフェクトのインスタンスハンドル

private: // 関数

	// 1人の対象を選択する
	void SelectSingleTarget(const BattleCharacterManager* bcm);

	// 全体の対象を選択する(決定ボタン押すだけ)
	void SelectAllTarget(const BattleCharacterManager* bcm);

	// 対象のステータスゲージを設定する
	void SetHealthGauge();


	// 行動を攻撃に設定する
	void SetBehaviourAttack(CommandBase* cmdBase);

	// 行動をアイテムに設定する
	void SetBehaviourUseItem(CommandBase* cmdBase);

	// 行動をスキルに設定する
	void SetBehaviourUseSkill(CommandBase* cmdBase);

public:

	// コンストラクタ
	DecideTargetChara(Character::Type characterType);

	// デストラクタ
	~DecideTargetChara() = default;

	// 初期化
	void Initialize(const BattleCharacterManager* bcm) override;

	// 更新
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;

	// 描画
	void Render();
};