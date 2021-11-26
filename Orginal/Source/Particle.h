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

// �p�[�e�B�N���Đ��N���X(���쒆) 
class Particle
{
	struct Vertex
	{
		Vector3 pos;
		Vector2 scale;
		Vector4 color;
		Vector4 param; // x:��] y:�A�j���^�C�}�[, z:x�R�}��, w:y�R�}��
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

	float mLifeTime; // ��������
	float mDuration; // �G�~�b�^�[�̗L������
	float mRateOverTime; // �� / �b(1�b�Ԃɉ��o����)
	float mEmitCount;

	Vector3 mEmitCenter; // �������S
	float mEmitRadius;  // �������a

	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	ConstantBuffer mConstBuffer;
	
	std::unique_ptr<Sprite> mSprite;
	TVector2<int> mSpriteNum;  // �R�}�̐�

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