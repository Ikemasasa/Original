#include "Particle.h"

#include "lib/Framework.h"
#include "lib/Sprite.h"

#include "GameManager.h"

void Particle::Animation(float animationPerSecond)
{
	float time = GameManager::elpsedTime;
	for (auto& data: mParticleData) 
	{
		if (!data.isEnable) continue;
		data.animeTimer += time * animationPerSecond;
	}
}

void Particle::Set(float timer, const Vector3& pos, const Vector3& velocity, const Vector2& scale)
{
	for (auto& data : mParticleData)
	{
		if (data.isEnable) continue;

		data.pos = pos;
		data.velocity = velocity;
		data.accel = Vector3::ZERO;
		data.scale = scale;
		
		data.color = Vector3::ONE;
		data.alpha = 1.0f;

		data.rotate = 0;

		data.timer = timer;
		data.animeTimer = 0.0f;
	}
}

void Particle::Initialize(int particleNum)
{
	D3D11_INPUT_ELEMENT_DESC elementDesc[] = {
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 0   , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SCALE"	 , 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"    , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PARAM"	 , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT elementDescNum = ARRAYSIZE(elementDesc);

	mShader = std::make_unique<Shader>();
	mShader->Load(L"Shaders/PointSprite.fx", "VSMain", "PSMain", elementDesc, elementDescNum);
	mShader->LoadGS(L"Shaders/PointSprite.fx", "GSMain");


	// バッファ作成
	ID3D11Device* device = FRAMEWORK.GetDevice();

	mParticleNum = particleNum;
	mVertices.resize(mParticleNum);
	mParticleData.resize(mParticleNum);

	// 頂点バッファ
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * mParticleNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, NULL, mVertexBuffer.GetAddressOf());

	// 定数バッファ
	mConstBuffer.Create(sizeof(CBForPerFrame));

	// テクスチャセット
	SetSprite(std::make_unique<Sprite>(L"Data/Image/Particle/particle_temp.png"), 1, 1);
}

void Particle::SetSprite(std::unique_ptr<Sprite> sprite, int xNum, int yNum)
{
	// ポインタ初期化
	mSprite.reset();

	mSprite.reset(sprite.release());
	mSpriteNum.x = xNum;
	mSpriteNum.y = yNum;
}

void Particle::Emitter(float duration, float rate, Vector3 center, float radius)
{
	mDuration = duration;
	mRateOverTime = rate;
	mLifeTime = 0;
	mEmitCount = 0;

	// TODO : 現在は球状にしか発生させられない
	mEmitCenter = center;
	mEmitRadius = radius;
}

void Particle::Update(float animationPerSecond)
{
	float time = GameManager::elpsedTime;

	// エミッター
	mLifeTime += time;
	mEmitCount += time;
	if (mLifeTime < mDuration)
	{
		float d = 1.0f / mRateOverTime;

		while (mEmitCount >= d)
		{
			float x = (rand() % 201 - 100) * 0.01f;
			float y = (rand() % 201 - 100) * 0.01f;
			float z = (rand() % 201 - 100) * 0.01f;
			float dist = sqrtf(x * x + y * y + z * z);

			x /= dist;
			y /= dist;
			z /= dist;

			dist = rand() % 100 * 0.01f * mEmitRadius;

			Vector3 pos;
			pos.x = x * dist + mEmitCenter.x;
			pos.y = y * dist + mEmitCenter.y;
			pos.z = z * dist + mEmitCenter.z;
							   
			Set(2.0f, pos,
				Vector3(0, 0, 0),// XMFLOAT3(rand() % 9 - 4, 6, 0.1f),
				Vector2(1,1)
			);

			mEmitCount -= d;
		}
	}

	Animation(animationPerSecond);
}

void Particle::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj)
{
	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// 定数バッファ更新
	CBForPerFrame cb;
	cb.view = view;
	cb.proj = proj;
	mConstBuffer.Update(&cb);
	mConstBuffer.Set(0);

	// 頂点更新
	int num = 0;
	for (auto& data : mParticleData)
	{
		if (!data.isEnable) continue;

		mVertices[num].pos = data.pos;
		mVertices[num].scale = data.scale;
		mVertices[num].color = data.color;
		mVertices[num].param.x = data.rotate;	  // 回転
		mVertices[num].param.y = data.animeTimer; // アニメタイマー
		mVertices[num].param.z = (float)mSpriteNum.x;	  // 連番画像の横の数
		mVertices[num].param.w = (float)mSpriteNum.y;    // 連番画像の縦の数
		++num;
	}
	context->UpdateSubresource(mVertexBuffer.Get(), 0, nullptr, mVertices.data(), 0, 0);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);

	mShader->Activate();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	Blend::Set(Blend::PMA);
	context->Draw(num, 0);
	Blend::Set(Blend::ALPHA);

	// レンダーステートを戻す
	FRAMEWORK.ResetParam();

}
