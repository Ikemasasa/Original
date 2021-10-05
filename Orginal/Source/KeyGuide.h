#pragma once

class KeyGuide final
{
	// À‘Ôì¬‹Ö~
	KeyGuide() = default;
	~KeyGuide() = default;

public:







	// ƒVƒ“ƒOƒ‹ƒgƒ“
	static KeyGuide& Instance()
	{
		static KeyGuide inst;
		return inst;
	}
};