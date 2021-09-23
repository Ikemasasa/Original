#include "NPCManager.h"

#include "lib/Math.h"

#include "CameraManager.h"
#include "DataBase.h"
#include "NPC.h"
#include "Singleton.h"

void NPCManager::Initialize()
{
	// çÏê¨
	mTalkIcon = std::make_unique<Sprite>(L"Data/Image/talk_icon.png");

	Create(DataBase::NPC_ID_START);

	for (auto& npc : mNPCs) npc->Initialize();
}

void NPCManager::Update()
{
	for (auto& npc : mNPCs) npc->Update();
}

void NPCManager::Render(const Matrix& view, const Matrix& proj, const Vector4& lightDir)
{
	for (auto& npc : mNPCs)
	{
		npc->Render(view, proj, lightDir);
	}
}

void NPCManager::Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir)
{
	for (auto& npc : mNPCs) npc->Render(shader, view, proj, lightDir);
}

void NPCManager::RenderUI()
{
	for (auto& npc : mNPCs)
	{
		Vector3 dist = mPlayerPos - npc->GetPos();
		if (dist.LengthSq() < (ICON_RENDER_DIST * ICON_RENDER_DIST))
		{
			float rate = Math::Clamp01(dist.LengthSq() / (ICON_RENDER_DIST * ICON_RENDER_DIST));
			float scale = Math::Lerp(1.0f, 0.3f, rate) * ICON_SCALE;
			const AABB& aabb = npc->GetAABB();

			const auto& view = Singleton<CameraManager>().GetInstance().GetView();
			const auto& proj = Singleton<CameraManager>().GetInstance().GetProj();
			Vector3 worldPos(aabb.min.x + (aabb.max.x - aabb.min.x) / 2.0f, aabb.max.y, aabb.min.z + (aabb.max.z - aabb.min.z) / 2.0f);
			Vector2 pos = worldPos.WorldToScreen(view, proj);
			Vector2 center(mTalkIcon->GetSize().x / 2.0f, mTalkIcon->GetSize().y);
			mTalkIcon->Render(pos, Vector2(scale, scale), Vector2::ZERO, mTalkIcon->GetSize(), center);
		}
	}
}

void NPCManager::Release()
{
	for (auto& npc : mNPCs) npc->Initialize();
}

void NPCManager::Create(const int charaID)
{
	int objID = mNPCs.size();
	mNPCs.emplace_back(std::make_unique<NPC>(charaID));
	mNPCs.back()->SetObjID(objID);
}
