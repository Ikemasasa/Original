#pragma once
#include <string>

#include "Sound.h"

class EffectData
{
public:
	struct Param
	{
		int slotID;
		std::u16string filename;
		float scale;
		float speed;
	};

	enum Kind
	{
		DEATH,
		HP_HEAL,
		MP_HEAL,
		BOMB,
		BUFF_ATK,
		DEBUFF_DEF,
		DAMAGE,
		SELECT_TARGET,
		COSMIC_MIST,

		NUM
	};

public:
	static EffectData::Param GetParam(EffectData::Kind kind);
};