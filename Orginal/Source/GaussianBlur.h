#pragma once
#include <memory>

#include "lib/Shader.h"
#include "lib/Vector.h"

class GaussianBlur
{
	static const int BUFFER_SIZE = 256;
	struct ConstBuffer
	{
		Vector4 weight[BUFFER_SIZE];
		float   karnelSize;
		Vector2 texcel;
		float   dummy;
	};

	std::unique_ptr<Shader> mShader;

public:
	GaussianBlur() = default;
	~GaussianBlur() = default;

};