#pragma once
#include <string>

class MotionCollision
{
	// ���ԍ쐬�֎~
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