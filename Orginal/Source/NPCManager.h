#pragma once
#include <DirectXMath.h>
#include <memory>
#include <vector>

#include "lib/Vector.h"

#include "NPC.h"
#include "NPCTalk.h"

class Shader;
class Sprite;

class NPCManager
{
public:
	static constexpr float TALK_DIST = 4.0f;
	static constexpr float TALK_DOT = 0.8f;

private:
	static constexpr float ICON_RENDER_DIST = 30.0f;
	static constexpr float ICON_SCALE = 0.125f;


	std::vector<std::unique_ptr<NPC>> mNPCs;
	std::unique_ptr<Sprite> mTalkIcon;
	Vector3 mPlayerPos;

public:
	NPCManager() = default;
	~NPCManager() = default;

	void Initialize();
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir);
	void Release();

	void Create(const int charaID);

	size_t GetNum() const { return mNPCs.size(); }
	NPC* GetNPC(size_t index) const { return mNPCs[index].get(); }
	void SetPlayerPos(const Vector3& pos) { mPlayerPos = pos; }
};