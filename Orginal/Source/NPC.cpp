#include "NPC.h"

#include "lib/Shader.h"
#include "lib/Math.h"

#include "CollisionTerrain.h"
#include "GameManager.h"
#include "TransformData.h"

NPC::NPC(int charaID) : Character(charaID, Character::NPC)
{

}

NPC::~NPC()
{

}

void NPC::Initialize()
{
	TransformData::Transform transform = TransformData::GetNPCTransform(GetCharaID());
	mPos = transform.pos;
	mScale = transform.scale;
	mAngle = transform.angle;

	SetCapsuleParam(transform.diameter / 2.0f);

	// �e�L�X�g�f�[�^�ǂݍ���
	NPCTextData::GetTextData(GetCharaID(), &mTextData, false);
	
	mTalkCount = TALK_COUNT_INIT;
	mOldFlag = false;

	SetMotion(Character::IDLE);
}

void NPC::Update()
{
	// �������̍��W�␳
	const float RAY_DIST = 1.0f;
	mPos.y = CollisionTerrain::GetHeight(mPos, RAY_DIST);

	// �{�X�����t���O�ƑO��̃t���O���`�F�b�N
	if (mOldFlag != GameManager::bossSubdueFlag)
	{
		mTextData.clear();
		NPCTextData::GetTextData(GetCharaID(), &mTextData, true);
		mTalkCount = TALK_COUNT_INIT;
		mOldFlag = GameManager::bossSubdueFlag;
	}

	UpdateWorld();
}

void NPC::Release()
{

}

std::vector<std::wstring> NPC::GetTexts()
{
	for (auto& textData : mTextData)
	{
		if (mTalkCount == textData.times)
		{
			mTalkCount = Math::Min(mTalkCount + 1, (int)mTextData.size());
			return textData.texts;
		}
	}

	// �ꉞ�����Ă���
	return std::vector<std::wstring>();
}

std::wstring NPC::GetName()
{	
	return mTextData[0].name;
}
