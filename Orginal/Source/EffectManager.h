#pragma once
#include <DirectXMath.h>

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>


#include "lib/Vector.h"

class CameraBase;

class EffectManager
{
	static const int SQUARE_MAX = 8000; // “K“–
	static const int EFFECT_MAX = 256; // “K“–
	static const int INST_MAX = 512;

private:
	Effekseer::ManagerRef mManager;
	EffekseerRendererDX11::RendererRef mRenderer;

	Effekseer::EffectRef mEffects[EFFECT_MAX] = { nullptr };
	int mEffectNum = 0;

	Effekseer::Handle mInstHandles[INST_MAX] = {-1};
	int mInstNum = 0;

public:
	EffectManager();
	~EffectManager();
	void SetProjectionMatrix(const DirectX::XMFLOAT4X4* projMatrix);

	int Create(const EFK_CHAR* efkPath); // –ß‚è’l : slot
	void Update();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj);

	int Play(const int& slot, const Vector3& pos, int startFrame = 0, float scale = 1.0f, float speed = 1.0f);
};