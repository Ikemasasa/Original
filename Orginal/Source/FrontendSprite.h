#pragma once

#include <memory>

#include "lib/Vector.h"

#include "lib/Sprite.h"

class FrontendSprite
{
	// �悭�悭�l����Ƃ��̃N���X���邩�E�E�E�H�Ǝv�������ǁA
	// �`��̃^�C�~���O�Ƃ�����ʂ�ł���ƕ֗���������Ȃ�
	// �R�X�g�Ɍ��������́E�E�E�ǂ����낤
	// �����Ƃ������@�͂��肻��

	static const int DATA_MAX = 64;

	struct Data
	{
		std::shared_ptr<Sprite> tex = nullptr;
		Vector2 pos					 = { 0.0f, 0.0f };
		Vector2 scale				 = { 0.0f, 0.0f };
		Vector2 texPos				 = { 0.0f, 0.0f };
		Vector2 size				 = { 0.0f, 0.0f };
		Vector2 center				 = { 0.0f, 0.0f };
		bool isEnable				 = false;
	};
	Data mData[DATA_MAX];

public:
	void Render();
	bool Set(const std::shared_ptr<Sprite>& tex, const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center);

};