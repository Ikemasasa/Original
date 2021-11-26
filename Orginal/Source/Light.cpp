#include "Light.h"

#include "lib/Framework.h"

void Light::CreateConstBuffer()
{
	mCB.Create(sizeof(CBForLight));
}

void Light::UpdateConstBuffer()
{
	CBForLight cb;
	cb.lightDir = Vector4(mLightDir, 1.0f);
	cb.lightColor = mLightColor;

	mCB.Update(&cb);
}

void Light::SetConstBuffer(int cbSlot)
{
	mCB.Set(cbSlot);
}

void Light::SetLightDir(const Vector3& lightDir)
{
	// ÉâÉCÉgï˚å¸ê≥ãKâª
	mLightDir = lightDir;
	mLightDir.Normalize();
}