#pragma once
#include <memory>

#include "lib/SkinnedMesh.h"

#include "MeshDataLoader.h"

/*
	メッシュを管理するクラス
	一応Singletonを通す
*/
class MeshManager
{
	static const int NAME_MAX = 128;
	std::vector<MeshDataLoader::MeshData> mMeshes;

public:
	std::shared_ptr<SkinnedMesh>& SearchLoad(int charaID);
	void Destory(const int charaID);
};