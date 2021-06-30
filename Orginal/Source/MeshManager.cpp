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

void MeshManager::Destory(const int charaID)
{
	for (auto it = mMeshes.begin(); it != mMeshes.end(); ++it)
	{
		if (charaID == it->charaID)
		{
			mMeshes.erase(it);
			break;
		}
	}
}
