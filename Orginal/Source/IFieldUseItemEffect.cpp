#include "IFieldUseItemEffect.h"

#include "Fade.h"
#include "SceneTitle.h"
#include "SceneManager.h"

void SubDueProofEffect::Execute()
{
	Fade::GetInstance().Set(Fade::SPEED_SLOW);
	if (Fade::GetInstance().IsFadeOutEnd())
	{
		SceneManager::GetInstance().SetNextScene(std::make_unique<SceneTitle>());
		mIsEffectFinished = true;
	}
}
