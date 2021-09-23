#pragma once
#include <DirectXmath.h>
#include <d3d11.h>
#include <fbxsdk.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <wrl.h>

#include "CollisionStructs.h"
#include "ComputeShader.h"
#include "Matrix.h"
#include "Shader.h"
#include "Sprite.h"
#include "Vector.h"

// いつかモーションの部分を別クラスにしたい

class SkinnedMesh
{
public:
	enum MotionType
	{
		DEFAULT,
		IDLE,
		WALK,
		RUN,
		ATTACK,
		GUARD,
		DIE,
		DAMAGE,
		USE_ITEM,
		UNIQUE1,
		UNIQUE2,
		UNIQUE3,

		MAX,
	};

private:
	static const int STR_MAX = 128;
	//static const int MOTION_MAX = 256;
	static const int BONE_MAX = 256;

	struct Cbuffer
	{
		Matrix wvp;		       // ワールド・ビュー・プロジェクション合成行列
		Matrix world;		   // ワールド変換行列
		Vector4 lightDir;	   // ライト方向
		Vector4 materialColor;
	};

	struct CbufferForSkinning
	{
		UINT groupNumX;
		UINT groupNumY;
		UINT groupNumZ;
		UINT verticesNum;
	};

	struct Vertex
	{
		Vector3 pos = {};
		Vector3 normal = {};
		Vector2 tex = {};
		Vector4 color = {};
		Vector3 tangent = {  };
		Vector3 binormal = { };
	};

	struct VertexForSkinning
	{
		Vector3 pos = {};
		Vector3 normal = {};
	};

	struct MaterialSprite
	{
		wchar_t filename[STR_MAX] = {};
		Sprite sprite;
	};

	struct Material
	{
		static const int KIND = 2;
		MaterialSprite mtlSpr[KIND];// Diffuse, normal
		int faceNum = 0;
	};

	struct Bone
	{
		char name[STR_MAX] = {};
		Matrix offsetMatrix = {};
		Matrix transform = {};
	};

	struct Weight
	{
		int count = 0;
		int bone[4] = {};
		float weight[4] = {};
	};

	struct Motion
	{
		int frameNum = 0;
		Matrix* keyframe[BONE_MAX] = {};

		// 現在は球だけ
		SPHERE collision;
		int colBeginFrame = 0;
		int colEndFrame = 0;
		bool isCollisionEnable = false;
	};

	struct BeforeMotion
	{
		int frame = 0; // 変わったタイミングのふれーむ
		Matrix keyframe[BONE_MAX][1] = {}; // 変わったタイミングのキーフレーム
		float lerpFactor = 1.0f;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>		mVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		mIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		mConstBuffer;
	std::unique_ptr<ComputeShader> mSkinningShader;
	std::unique_ptr<Shader> mShader;


	char mFbxDir[STR_MAX];
	char mBinFile[STR_MAX];

	int mMeshNum = 0;
	int mBoneNum = 0;
	int mVerticesNum = 0;
	int mFaceNum;
	Bone mBones[BONE_MAX];
	Vertex* mVertices;
	VertexForSkinning* mVerticesSource;
	u_int* mIndices;
	Material* mMaterials;
	Vector4* mMaterialColors;
	Weight* mWeights;

	void Initialize(const char* fbxFilename);
	void LoadFBX(ID3D11Device* device, const char* filename);
	bool LoadBIN(const char* filename);
	void Save(const char* filename/*バイナリファイル名*/);

	void LoadBone(FbxMesh* mesh);
	int FindBone(const char* name);
	void OptimizeVertices();

	// アニメーション関係
	int mStartFrame;
	bool mIsLoop;
	bool mMotionFinished;
	float mFrame = 0;
	MotionType mMotionType = DEFAULT; // 現在のモーション;
	std::map<MotionType, Motion> mMotion; // モーションデータ

	void LoadKeyFrames(MotionType type, int bone, FbxNode* boneNode);
	void LoadMeshAnim(FbxMesh* mesh); // ボーンがないメッシュアニメ―ション


public:
	SkinnedMesh(const char* fbxFilename);
	virtual ~SkinnedMesh();

	void Render(const Matrix& wvp, const Matrix& world, const Vector4& lightDir, float elapsedTime, float alpha = 1.0f);
	void Render(const Shader* shader, const Matrix& wvp, const Matrix& world, const Vector4& lightDir, float elapsedTime, float alpha = 1.0f);

	int RayPick(
		const Vector3& pos,
		const Vector3& velocity,
		Vector3* outPos,
		Vector3* outNormal);

	void ChangeShader(Shader* shader);

	// モデルぴったりの直方体(ローカル)
	AABB mAABB;

	void SetMotion(MotionType type, bool isLoop = false);
	MotionType GetMotion() const { return mMotionType; }
	void AddMotion(const char* filename, MotionType type);
	void Animate(float elapsedTime);
	void Skinning(); // ボーンによる変形

	bool IsMotionFinished() const { return mMotionFinished; }
};