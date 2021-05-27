#include "MeshManager.h"

std::shared_ptr<SkinnedMesh>& MeshManager::SearchLoad(int charaID)
{
	// すでに読み込んでいるかチェック
	for (auto& mesh : mMeshes)
	{
		if (mesh.charaID == charaID)
		{
			return mesh.mesh;
		}
	}

	// 読み込み
	mMeshes.emplace_back(MeshDataLoader::LoadMesh(charaID));

	return mMeshes.back().mesh;
}

void MeshManager::Update()
{
	for (auto it = mMeshes.begin(); it != mMeshes.end();)
	{
		if (it->mesh.use_count() == 1) // 所有権が自分だけなら
		{
			mMeshes.erase(it);
			continue;
		}

		++it;
	}
}
