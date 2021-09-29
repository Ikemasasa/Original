#pragma once
#include <vector>

#include "EquipmentInventory.h"
#include "Item.h"

class CameraBase;
class Matrix;
class Player;
class Shader;
class Vector4;

class PlayerManager
{
	static const int INVINCIBLE_SECOND = 2;


public:
	enum PlList
	{
		ERIKA = DataBase::PL_ID_START,
		PALADIN,

		PL_ID_MAX
	};
	static const int ALL_PLAYER_NUM = PL_ID_MAX - DataBase::PL_ID_START;

private:
	std::vector<std::unique_ptr<Player>> mPlayers;
	Player* mMovePlayer = nullptr;
	bool mIsInvincible = false;
	float mInvincibleTimer = 0;

	EquipmentInventory mEquipmentInventory;

public:
	PlayerManager();
	~PlayerManager();

	void Create(int charaID);

	void Initialize();
	void Update(const bool isTalking);
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// ゲッター
	Player* GetMovePlayer() const { return mMovePlayer; }
	Player* GetPlayer(const size_t index) const { return mPlayers[index].get(); }
	size_t GetNum() const { return mPlayers.size(); }
	bool IsInvincible() const { return mIsInvincible; }
	EquipmentInventory* GetEquipmentInventory() { return &mEquipmentInventory; }

	// セッター
	void EnableInvincible() { mIsInvincible = true; }

};