#pragma once
#include <vector>

#include "Item.h"
#include "Player.h"

class CameraBase;

class PlayerManager
{
public:
	enum PlList
	{
		SOPHIE = DataBase::PL_ID_START,
	};

private:
	std::vector<std::unique_ptr<Player>> mPlayers;
	Player* mMovePlayer = nullptr;

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
	Player* GetMovePlayer() { return mMovePlayer; }
	Player* GetPlayer(const size_t index) const { return mPlayers[index].get(); }
	size_t GetNum() const {return mPlayers.size(); }
};