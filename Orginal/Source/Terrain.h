#pragma once
#include <memory>

#include "Object.h"

class Terrain : public Object
{
public:
	Terrain(int charaID);
	~Terrain() = default;

	void Initialize();
};