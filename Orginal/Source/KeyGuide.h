#pragma once

class KeyGuide final
{
	// ���ԍ쐬�֎~
	KeyGuide() = default;
	~KeyGuide() = default;

public:







	// �V���O���g��
	static KeyGuide& Instance()
	{
		static KeyGuide inst;
		return inst;
	}
};