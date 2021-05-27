#pragma once
#include <memory>

#include "Actor.h"

#include "lib/Vector.h"

class Terrain : public Actor
{
public:
	Terrain(int charaID);
	~Terrain() = default;

	void Initialize();
};