#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Actor.h"
//
//template<class T>
//class ManagerBase
//{
////protected: // ˆê‰ž
////	std::vector<std::shared_ptr<T>> mActors;
////
////public:
////	void Create(int charaID)
////	{
////
////	}
////	void Destory(int objId);
////	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir, const float elapsedTime);
////	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4& lightDir, const float elapsedTime);
////	std::shared_ptr<Actor> GetActor(int objID);
////
////	size_t GetActorNum() { return mActorNum; }
//};