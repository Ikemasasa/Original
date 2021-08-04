#pragma once
#include <DirectXMath.h>
#include <vector>

#include "Item.h"

class CameraBase;
class Player;
class Shader;

class PlayerManager
{
	static const int INVINCIBLE_SECOND = 2;

public:
	enum PlList
	{
		PLAYER_A = DataBase::PL_ID_START,
		PLAYER_B,

		PL_NUM
	};

private:
	std::vector<std::unique_ptr<Player>> mPlayers;
	Player* mMovePlayer = nullptr;
	bool mIsInvincible = false;
	float mInvincibleTimer = 0;


	//std::vector<EquipmentData::Param> mEquipments;
public:
	PlayerManager() = default;
	~PlayerManager() = default;

	void Create(int charaID);

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir);

	// ゲッター
	Player* GetMovePlayer() const { return mMovePlayer; }
	Player* GetPlayer(const size_t index) const { return mPlayers[index].get(); }
	size_t GetNum() const {return mPlayers.size(); }
	bool IsInvincible() const { return mIsInvincible; }

	// セッター
	void EnableInvincible() { mIsInvincible = true; }

};