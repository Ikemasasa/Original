#pragma once
#include "SceneBase.h"
#include "lib/Texture.h"

class SceneTitle : public SceneBase
{
	// とりあえず一枚絵のため
	Texture* mSprite;

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
};

