#pragma once

class PlayerManager;

class MenuBase
{
	friend class MenuManager;

protected:
	enum Select
	{
		ITEM,
		STATUS,

		SELECT_NUM,
		NONE = 10 // �A�C�e��(�X�e�[�^�X�ł���������)����ԏ�ɂ��邽�߂�NONE����ԍŌ�ɂ��Ă�
	};

	int mSelectIndex = 0;

public:
	MenuBase() = default;
	virtual ~MenuBase() = default;

	virtual Select Update(PlayerManager* plm) = 0;
	virtual void Render() = 0;
};