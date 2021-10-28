#pragma once
#include "CameraBase.h"

#include <memory>

#include "lib/Vector.h"

class Character;

class CameraBattle : public CameraBase
{
	static constexpr float COMMAND_SELECT_DIST = 7.0f;
	static constexpr float ATTACK_DIST = 6.0f;
	static constexpr float LERP_SPEED = 0.4f;

	static constexpr float LERP_START_FADE_ALPHA = 0.6f;

	float mTimer = 0;

	Vector3 mBeginPos;
	Vector3 mEndPos;
	float mLerpVelocity = 0.0f;
	float mLerpFactor = 0.0f;

	Vector3 mCommandSelectPos = {};

	bool mPlAttackFinishFlag = false;
	bool mEnmAttackFinishFlag = false;

private:
	void Begin();
	void CommandSelect(const BattleCharacter* commandChara);
	void PartySelect(const BattleCharacter* commandChara);
	void EnemySelect(const BattleCharacter* commandChara);
	void PlayerAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm);
	void EnemyAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm);
	void Escape(const BattleCharacter* commandChara);

public:
	CameraBattle();
	~CameraBattle() = default;
	void Update(const BattleCharacter* commandChara, const BattleCharacterManager* bcm) override;
};