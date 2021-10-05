#pragma once

class KeyGuide final
{
	// 実態作成禁止
	KeyGuide() = default;
	~KeyGuide() = default;

public:







	// シングルトン
	static KeyGuide& Instance()
	{
		static KeyGuide inst;
		return inst;
	}
};