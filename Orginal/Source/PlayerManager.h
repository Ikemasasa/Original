#pragma once
#include <vector>

#include "Item.h"
#include "Player.h"
#include "ManagerBase.h"

class CameraBase;

class PlayerManager
{
public:
	enum PlList
	{
		SOPHIE = DataBase::PL_ID_START,
	};

private:
	std::vector<std::shared_ptr<Player>> mPlayers;
	std::shared_ptr<Player> mMovePlayer = nullptr;

public:
	PlayerManager() = default;
	~PlayerManager() = default;

	void Create(int charaID);
	void Destroy(int objID);

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir);

	// ÉQÉbÉ^Å[
	std::shared_ptr<Player> GetMovePlayer() { return mMovePlayer; }
	const std::vector<std::shared_ptr<Player>>& GetPlayers() const { return mPlayers; }
};