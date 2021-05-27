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
#include "Shader.h"
#include "Texture.h"


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
		DIE,
		UNIQUE1,
		UNIQUE2,
		UNIQUE3,

		MAX
	};

private:
	static const int STR_MAX = 128;
	//static const int MOTION_MAX = 256;
	static const int BONE_MAX = 256;
	DirectX::XMFLOAT4X4 coordConversion = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	bool mIsSwapYZ = false;

	struct Cbuffer
	{
		DirectX::XMFLOAT4X4 wvp; // ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4 world;				   // ワールド変換行列
		DirectX::XMFLOAT4 lightDirection;		   // ライト進行方向
		DirectX::XMFLOAT4 materialColor;
	};

	struct Vertex
	{
		DirectX::XMFLOAT3 pos = {};
		DirectX::XMFLOAT3 normal = {};
		DirectX::XMFLOAT2 tex = {};
		DirectX::XMFLOAT4 color = {};
	};

	struct VertexForSkinning
	{
		DirectX::XMFLOAT3 pos = {};
		DirectX::XMFLOAT3 normal = {};
	};

	struct Material
	{
		wchar_t filename[STR_MAX] = {};
		Texture diffuse;
		Texture normal;
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
	};

	struct BeforeMotion
	{
		int frame = 0; // 変わったタイミングのふれーむ
		Matrix keyframe[BONE_MAX][1] = {}; // 変わったタイミングのキーフレーム
		float lerpFactor = 1.0f;
	};

	struct Face
	{
		DirectX::XMFLOAT3 vertex[3];
		int materialIndex;
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>		mVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		mIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		mConstBuffer;
	std::unique_ptr<ComputeShader> mSkinningShader;
	Shader mShader;


	char mFbxDir[STR_MAX];
	char mBinFile[STR_MAX];

	int mMeshNum = 0;
	int mBoneNum = 0;
	int mVerticesNum = 0;
	Bone mBones[BONE_MAX];
	std::vector<Vertex>				 mVertices;
	std::vector<VertexForSkinning>   mVerticesSource;
	std::vector<u_int>	  mIndices;
	std::vector<Material> mMaterials;
	std::vector<Vector4>  mMaterialColors;
	std::vector<Weight>	  mWeights;
	std::vector<Face>	  mFaces;

	void Initialize(const char* fbxFilename);
	void LoadFBX(ID3D11Device* device, const char* filename);
	bool LoadBIN(const char* filename);
	void Save(const char* filename/*バイナリファイル名*/);

	void LoadBone(FbxMesh* mesh);
	int FindBone(const char* name);
	void OptimizeVertices();

	// アニメーション関係
	int mStartFrame;
	int mMotionNum; //モーション数
	bool mIsLoop;
	BeforeMotion mBeforeMotion; // 変更前のモーション
	std::map<MotionType, Motion> mMotion; // モーションデータ
	void LoadKeyFrames(MotionType type, int bone, FbxNode* boneNode);
	void Skinning(); // ボーンによる変形
	void LoadMeshAnim(FbxMesh* mesh); // ボーンがないメッシュアニメ―ション


public:
	SkinnedMesh(const char* fbxFilename);
	virtual ~SkinnedMesh();

	void Render(
		const DirectX::XMFLOAT4X4& wvp, // ワールド・ビュー・プロジェクション合成行列
		const DirectX::XMFLOAT4X4& world, // ワールド変換行列
		const DirectX::XMFLOAT4& lightDirection,	 // ライト進行方向
		float elapsedTime,
		float alpha = 1.0f
	);

	int RayPick(
		const DirectX::XMFLOAT3& sp,
		const DirectX::XMFLOAT3& ep,
		DirectX::XMFLOAT3* outPos,
		DirectX::XMFLOAT3* outNormal,
		float* outLen);

	// モデルぴったりの直方体(ローカル)
	AABB mAABB;

	// モーション情報
	float mFrame = 0;
	MotionType mMotionType; // 現在のモーション;

	void SetMotion(MotionType type, bool isLoop = false);
	MotionType GetMotion() const { return mMotionType; }
	void AddMotion(const char* filename, MotionType type);
	void Animate(float elapsedTime);
};