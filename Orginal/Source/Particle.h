#pragma once
#include <memory>
#include <vector>
#include <wrl.h>

#include "lib/ConstantBuffer.h"
#include "lib/Shader.h"
#include "lib/Vector.h"
#include "lib/TVector.h"
#include "lib/Blend.h"



class Sprite;

// パーティクル再生クラス(制作中) 
class Particle
{
	struct Vertex
	{
		Vector3 pos;
		Vector2 scale;
		Vector4 color;
		Vector4 param; // x:回転 y:アニメタイマー, z:xコマ数, w:yコマ数
	};

	struct ParticleData
	{
		bool isEnable;

		Vector3 pos;
		Vector3 velocity;
		Vector3 accel;
		Vector2 scale;
		float rotate;
		Vector3 color;
		float alpha;

		float timer;

		float animeTimer;
	};

	struct CBForPerFrame
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 proj;
	};

	std::vector<Vertex> mVertices;
	std::vector<ParticleData> mParticleData;
	int mParticleNum;

	float mLifeTime; // 生存時間
	float mDuration; // エミッターの有効時間
	float mRateOverTime; // 個 / 秒(1秒間に何個出すか)
	float mEmitCount;

	Vector3 mEmitCenter; // 発生中心
	float mEmitRadius;  // 発生半径

	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	ConstantBuffer mConstBuffer;
	
	std::unique_ptr<Sprite> mSprite;
	TVector2<int> mSpriteNum;  // コマの数

	std::unique_ptr<Shader> mShader;


private:
	void Animation(float animationPerSecond);
	void Set(float timer, const Vector3& pos, const Vector3& velocity, const Vector2& scale);

public:
	void Initialize(int particleNum);
	void SetSprite(std::unique_ptr<Sprite> sprite, int xNum, int yNum);

	void Emitter(float duration, float rate, Vector3 center, float radius);

	void Update(float animationPerSecond);
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj);
};