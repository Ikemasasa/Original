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
	static constexpr float TALK_DIST = 8.0f;
	static constexpr float TALK_DOT = 0.5f;

private:
	static constexpr float ICON_RENDER_DIST = TALK_DIST * 2.0f;
	static constexpr float ICON_SCALE = 0.125f;


	std::vector<std::unique_ptr<NPC>> mNPCs;
	std::unique_ptr<Sprite> mTalkIcon;
	Vector3 mPlayerPos = {};

public:
	NPCManager() = default;
	~NPCManager() = default;

	void Initialize();
	void Update();
	void Render(const Shader* shader, const Matrix& view, const Matrix& projection, const Vector4& lightDir);
	void RenderUI();
	void Release();

	void Create(const int charaID);

	size_t GetNum() const { return mNPCs.size(); }
	NPC* GetNPC(size_t index) const { return mNPCs[index].get(); }
	void SetPlayerPos(const Vector3& pos) { mPlayerPos = pos; }
};