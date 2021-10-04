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

void Light::SetLightDir(Vector3& lightDir, const Vector3& lightPos)
{
	// ライト方向正規化
	lightDir.Normalize();
	mLightDir = Vector4(lightDir, 1.0f);

	// ライトの方向と座標を合わせる
	float xSign = (lightDir.x > 0) ?  1 :-1;
	float ySign = (lightDir.y > 0) ? -1 : 1;
	float zSign = (lightDir.z > 0) ?  1 :-1;
	mLightPos.x = lightPos.x * xSign;
	mLightPos.y = lightPos.y * ySign;
	mLightPos.z = lightPos.z * zSign;
}