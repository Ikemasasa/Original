#pragma once

#include "lib/Vector.h"

class TransformData
{
	// ���ԍ쐬�֎~
	TransformData() = default;
	~TransformData() = default;
	
public:
	struct Transform
	{
		Vector3 pos;
		Vector3 scale;
		Vector3 angle;
		float diameter;
	};

public:
	static Transform GetPLTransform(const int charaID);
	static Transform GetEnmTransform(const int charaID);
	static Transform GetNPCTransform(const int charaID);
};