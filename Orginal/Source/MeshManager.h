#pragma once
#include <memory>

#include "lib/SkinnedMesh.h"

#include "MeshDataLoader.h"

// Singleton‚ð’Ê‚·
class MeshManager
{
	static const int NAME_MAX = 128;
	std::vector<MeshDataLoader::MeshData> mMeshes;

public:
	std::shared_ptr<SkinnedMesh>& SearchLoad(int charaID);
	void Update();
};