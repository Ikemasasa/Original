#pragma once
#include "SceneBase.h"
#include "lib/Texture.h"

class SceneTitle : public SceneBase
{
	// �Ƃ肠�����ꖇ�G�̂���
	Texture* mSprite;

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
};

