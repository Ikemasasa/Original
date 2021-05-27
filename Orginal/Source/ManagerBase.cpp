//#include "ManagerBase.h"
//
//void ManagerBase::Create(int charaID)
//{
//	std::shared_ptr<Actor> actor = std::make_shared<Actor>(charaID);
//
//	int objID = mActorNum++;
//	actor->SetObjID(objID);
//	mActors[objID] = actor;
//}
//
//void ManagerBase::Destory(int objId)
//{
//	for (int i = 0; i < ACTOR_MAX; ++i)
//	{
//		if (mActors[i]->GetObjID() != objId) continue;
//
//		mActors[i] = nullptr;
//		--mActorNum;
//	}
//}
//
//void ManagerBase::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir, const float elapsedTime)
//{
//	for (int i = 0; i < ACTOR_MAX; ++i)
//	{
//		std::shared_ptr<Actor>& target = mActors[i];
//		if (target)
//		{
//			if (target->GetExist())
//			{
//				target->Render(view, proj, lightDir, elapsedTime);
//			}
//		}
//	}
//}
//
//void ManagerBase::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir, const float elapsedTime)
//{
//	for (int i = 0; i < ACTOR_MAX; ++i)
//	{
//		std::shared_ptr<Actor>& target = mActors[i];
//		if (target)
//		{
//			if (target->GetExist())
//			{
//				target->Render(shader, view, proj, lightDir, elapsedTime);
//			}
//		}
//	}
//}
//
//std::shared_ptr<Actor> ManagerBase::GetActor(int objID)
//{
//	for (int i = 0; i < ACTOR_MAX; ++i)
//	{
//		if (mActors[i]->GetObjID() != objID) continue;
//
//		return mActors[i];
//	}
//
//	return nullptr; // Ž¸”s
//}
