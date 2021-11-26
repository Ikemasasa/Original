#pragma once
#include <list>
#include <memory>

#include "lib/Font.h"
#include "lib/Vector.h"

class Sprite;

// �h���b�v�A�C�e����\������N���X
class DropItemShower
{
	// �萔
	static constexpr float VELOCITY_Y = 0.5f;
	static constexpr float ALPHA_ADD = 0.1f;

	static constexpr float DISP_SEC = 5.0f;

	static constexpr float STR_OFFSET_Y = 5.0f;

private: // �ϐ�
	struct Data
	{
		int itemID;
		Vector2 pos;
		float alpha;
		float alphaAdd;
		float dispTimer;
		bool fade;
	};

	std::list<Data> mDropItemDatas;
	std::unique_ptr<Sprite> mBoard;
	Font mFont;

public: // �֐�

	// �R���X�g���N�^
	DropItemShower();
	
	// �f�X�g���N�^
	~DropItemShower() = default;

	// �h���b�v�A�C�e����ǉ�(�\���ɓo�^)
	void Add(const int dropItemID, const Vector3& dropCharaPos);

	void Update();
	void Render();
};