#include "MeshManager.h"

std::shared_ptr<SkinnedMesh>& MeshManager::SearchLoad(int charaID)
{
	// ���łɓǂݍ���ł��邩�`�F�b�N
	for (auto& mesh : mMeshes)
	{
		if (mesh.charaID == charaID)
		{
			return mesh.mesh;
		}
	}

	// �ǂݍ���
	mMeshes.emplace_back(MeshDataLoader::LoadMesh(charaID));

	return mMeshes.back().mesh;
}

void MeshManager::Update()
{
	for (auto it = mMeshes.begin(); it != mMeshes.end();)
	{
		if (it->mesh.use_count() == 1) // ���L�������������Ȃ�
		{
			mMeshes.erase(it);
			continue;
		}

		++it;
	}
}
