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

void Light::SetLightDir(const Vector4& lightDir)
{
	// ê≥ãKâª
	Vector3 normalize(lightDir.x, lightDir.y, lightDir.z);
	normalize.Normalize();

	mLightDir = Vector4(normalize.x, normalize.y, normalize.z, lightDir.w);
}