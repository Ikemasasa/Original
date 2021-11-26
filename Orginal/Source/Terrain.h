#pragma once
#include <memory>

#include "Object.h"

// 地形クラス
class Terrain : public Object
{
public:
	
	// コンストラクタ
	Terrain(int charaID);

	// デストラクタ
	~Terrain() = default;

	// 初期化
	void Initialize();
};