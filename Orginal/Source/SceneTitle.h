#pragma once
#include "SceneBase.h"
#include "lib/Texture.h"

class SceneTitle : public SceneBase
{
	// ‚Æ‚è‚ ‚¦‚¸ˆê–‡ŠG‚Ì‚½‚ß
	Texture* mSprite;

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
};

