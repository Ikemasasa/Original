#pragma once

#include "lib/CollisionStructs.h"

#include "EffectData.h"
#include "ProductionValue.h"
#include "Sound.h"

class BattleCharacter;
class BattleCharacterManager;

class IBattleProduction
{
protected: // 文字の色
	static const Vector3 DAMAGE_RGB;
	static const Vector3 HEAL_HP_RGB;
	static const Vector3 HEAL_MP_RGB;
	static constexpr float DEFAULT_WAIT_TIME = 0.8f;

protected:
	BattleCharacter* mMoveChara = nullptr;	 // IDから取得したのを保存するよう
	std::vector<BattleCharacter*> mTargetCharas; // 同上
	std::vector<BattleCharacter*> mHitChara; // コリジョン用
	std::vector<int> mHPAmounts;
	std::vector<int> mMPAmounts;
	std::vector<int> mEffectInstHandles;
	ProductionValue mProductionValue;
	bool mIsFinished = false;
	float mWaitTimer = 0.0f;

protected:
	bool StateWait(const float& waitTime = DEFAULT_WAIT_TIME);

public:
	IBattleProduction() = default;
	virtual ~IBattleProduction() = default;

	void Begin(BattleCharacter* moveChara, std::vector<BattleCharacter*>& targetCharas);
	void DeathProduction();
	void ValueProduction();
	void HitProduction(const SPHERE& sphere, const EffectData::Kind& effect, const Sound::Kind& sound);
	void HealProduction(const EffectData::Kind& effect, const Sound::Kind& sound);
	void BuffProduction(const EffectData::Kind& effect, const Sound::Kind& sound);
	void AtkSkillProduction(const EffectData::Kind& effect, const Sound::Kind& sound);


	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() {};

	bool IsFinished() const { return mIsFinished; }
};