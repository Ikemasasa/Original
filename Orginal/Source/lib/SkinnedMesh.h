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

// �������[�V�����̕�����ʃN���X�ɂ�����

class SkinnedMesh
{
private:
	static const int STR_MAX = 128;
	//static const int MOTION_MAX = 256;
	static const int BONE_MAX = 256;
	static const int T_POSE_INDEX = 0;

	struct Cbuffer
	{
		Matrix wvp;		       // ���[���h�E�r���[�E�v���W�F�N�V���������s��
		Matrix world;		   // ���[���h�ϊ��s��
		Vector4 lightDir;	   // ���C�g����
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

		// ���݂͋�����
		SPHERE collision;
		int colBeginFrame = 0;
		int colEndFrame = 0;
		bool isCollisionEnable = false;
	};

	struct BeforeMotion
	{
		int frame = 0; // �ς�����^�C�~���O�̂ӂ�[��
		Matrix keyframe[BONE_MAX][1] = {}; // �ς�����^�C�~���O�̃L�[�t���[��
		float lerpFactor = 1.0f;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>		mVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		mIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		mConstBuffer;
	std::unique_ptr<ComputeShader> mSkinningShader;


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


	// �A�j���[�V�����֌W
	int mStartFrame;
	bool mIsLoop;
	bool mMotionFinished;
	float mFrame = 0;
	float mBeforeFrame = 0; // �O�̃��[�V�����̃t���[��
	float mBlendFactor = 1.0f;
	float mBlendSpeed = 0.1f;
	int mMotionType = 0; // ���݂̃��[�V����
	int mNextType = 0;	 // ���[�v���Ȃ����[�V�����̎��̃��[�V����
	int mBeforeMotionType = 0; // �O�̃��[�V����
	std::map<int, Motion> mMotion; // ���[�V�����f�[�^
private:

	void Initialize(const char* fbxFilename);
	void LoadFBX(ID3D11Device* device, const char* filename);
	bool LoadBIN(const char* filename);
	void Save(const char* filename/*�o�C�i���t�@�C����*/);

	void LoadBone(FbxMesh* mesh);
	int FindBone(const char* name);
	void OptimizeVertices();

	void Animate(float elapsedTime);

	void LoadKeyFrames(int type, int bone, FbxNode* boneNode);
	void LoadMeshAnim(FbxMesh* mesh); // �{�[�����Ȃ����b�V���A�j���\�V����


public:
	SkinnedMesh(const char* fbxFilename);
	virtual ~SkinnedMesh();

	void Render(const Shader* shader, const Matrix& wvp, const Matrix& world, const Vector4& lightDir);
	int RayPick(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);

	// ���f���҂�����̒�����(���[�J��)
	AABB mAABB;

	void SetMotion(int type, float blendSpeed);
	void SetMotionOnce(int noLoopType, int nextType, float blendSpeed);
	int GetMotion() const { return mMotionType; }
	void AddMotion(const char* filename, int type);

	void Skinning(float elapsedTime); // �{�[���ɂ��ό`

	bool IsMotionFinished() const { return mMotionFinished; }
};