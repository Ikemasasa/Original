#pragma once
#include <memory>

#include "Object.h"

// �n�`�N���X
class Terrain : public Object
{
public:
	
	// �R���X�g���N�^
	Terrain(int charaID);

	// �f�X�g���N�^
	~Terrain() = default;

	// ������
	void Initialize();
};