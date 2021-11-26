#pragma once
#include<memory>

#include "lib/SkinnedMesh.h"

// メッシュを読み込むクラス
class MeshDataLoader
{
public:
	struct MeshData
	{
		std::shared_ptr<SkinnedMesh> mesh;
		int charaID;
	};

	static MeshData LoadMesh(int charaID);
};