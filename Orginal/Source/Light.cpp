#include "Light.h"

#include "lib/Framework.h"

void Light::CreateConstBuffer()
{
	mCB.Create(sizeof(CBForLight));
}

void Light::UpdateConstBuffer()
{
	CBForLight cb;
	cb.lightDir = mLightDir;
	cb.lightColor = mLightColor;

	mCB.Update(&cb);
}

void Light::SetConstBuffer(int cbSlot)
{
	mCB.Set(cbSlot);
}
