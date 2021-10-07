#pragma once
#include <string>

class MotionCollision
{
	// é¿ë‘çÏê¨ã÷é~
	MotionCollision() = default;
	~MotionCollision() = default;


public:
	struct ColData
	{
		std::string boneName;
		float radius;
		int beginFrame;
		int endFrame;
	};

public:
	static ColData GetPLMotionCollision(const int charaID);
	static ColData GetEnmMotionCollision(const int charaID);
};