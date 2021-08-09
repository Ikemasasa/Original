#pragma once

class PlayerManager;

class MenuBase
{
	friend class MenuManager;

protected:
	enum Select
	{
		ITEM,
		EQUIPMENTS,
		STATUS,
		SELECT_NUM,

		BACK,
		NONE = 10 // �A�C�e��(�X�e�[�^�X�ł���������)����ԏ�ɂ��邽�߂�NONE����ԍŌ�ɂ��Ă�
	};

	int mSelectIndex = 0;

public:
	MenuBase() = default;
	virtual ~MenuBase() = default;

	virtual void Initialize(const PlayerManager* plm) = 0;
	virtual Select Update(PlayerManager* plm) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};