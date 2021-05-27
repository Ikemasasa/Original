#include "SkinnedMesh.h"

#include "Framework.h"
#include "Math.h"
#include "ResourceManager.h"


using namespace fbxsdk;

void SkinnedMesh::LoadFBX(ID3D11Device* device, const char* filename)
{	
	// �œK���t�@�C���`�F�b�N
	char name2[STR_MAX];
	strcpy_s(name2, STR_MAX, filename);
	name2[strlen(name2) - 3] = 'B';
	name2[strlen(name2) - 2] = 'I';
	name2[strlen(name2) - 1] = 'N';

	
	strcpy_s(mBinFile, STR_MAX, name2);
	if (LoadBIN(name2))return;

	FbxManager* manager = FbxManager::Create();

	// �t�@�C������V�[���ɓǂݍ���
	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename);
	FbxScene* scene = FbxScene::Create(manager, "");
	importer->Import(scene);
	

	FbxAxisSystem axis = scene->GetGlobalSettings().GetAxisSystem();
	int sign = 1;
	fbxsdk::FbxAxisSystem::EUpVector up = axis.GetUpVector(sign);
	if (up != fbxsdk::FbxAxisSystem::EUpVector::eYAxis)
	{
		coordConversion = { 1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1 };
		mIsSwapYZ = true;
	}

	//// ���[�V�������擾
	//FbxArray<FbxString*> names;
	//scene->FillAnimStackNameArray(names);
	int animStackCount = importer->GetAnimStackCount();
	if (animStackCount > 0)
	{
		// ���[�V���������݂���Ƃ�
		FbxTakeInfo* take = importer->GetTakeInfo(0);
		FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
		FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
		FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
		mStartFrame = (int)(start / fps60);
		mMotion[DEFAULT].frameNum = (int)((stop - start) / fps60);
		SetMotion(DEFAULT);
	}
	else
	{
		mStartFrame = 0;
		mMotion[DEFAULT].frameNum = 0;
	}
	
	importer->Destroy();

	// ���f����3�p�`�ɕ���
	fbxsdk::FbxGeometryConverter geometryConverter(manager);
	geometryConverter.SplitMeshesPerMaterial(scene, true);
	geometryConverter.Triangulate(scene, true);

	// ���b�V�����擾
	mMeshNum = scene->GetSrcObjectCount<fbxsdk::FbxMesh>();

	// ���_���v�Z
	int vertexNum = 0; // ���v���_��
	for (int i = 0; i < mMeshNum; ++i)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(i);
		int num = mesh->GetPolygonVertexCount();
		vertexNum += num;
	}

	// ���_�m��
	mVertices.resize(vertexNum);
	mIndices.resize(vertexNum);
	mWeights.resize(vertexNum);

	// �}�e���A�����Ƃ̃|���S�����_��
	mMaterials.resize(mMeshNum);
	mMaterialColors.resize(mMeshNum);

	// ���_�ǂݍ���
	mVerticesNum = 0;
	mBoneNum = 0;
	for (int m = 0; m < mMeshNum; ++m)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(m);
		int num = mesh->GetPolygonVertexCount();
		
		// ���W
		int* index = mesh->GetPolygonVertices();
		FbxVector4* source = mesh->GetControlPoints();

		FbxAMatrix globalTrans = mesh->GetNode()->EvaluateGlobalTransform();
		// �S���_�ϊ�
		for (int v = 0; v < mesh->GetControlPointsCount(); ++v)
		{
			source[v] = globalTrans.MultT(source[v]);
		}

		// ���_���W�ǂݍ���
		for (int v = 0; v < num; ++v)
		{
			int vIndex = index[v];

			mVertices[v + mVerticesNum].pos.x = (float)source[vIndex][0];
			mVertices[v + mVerticesNum].pos.y = (float)source[vIndex][1];
			mVertices[v + mVerticesNum].pos.z = (float)source[vIndex][2];

			mAABB.min.x = Math::Min(mAABB.min.x, mVertices[v + mVerticesNum].pos.x);
			mAABB.min.y = Math::Min(mAABB.min.y, mVertices[v + mVerticesNum].pos.y);
			mAABB.min.z = Math::Min(mAABB.min.z, mVertices[v + mVerticesNum].pos.z);

			mAABB.max.x = Math::Max(mAABB.max.x, mVertices[v + mVerticesNum].pos.x);
			mAABB.max.y = Math::Max(mAABB.max.y, mVertices[v + mVerticesNum].pos.y);
			mAABB.max.z = Math::Max(mAABB.max.z, mVertices[v + mVerticesNum].pos.z);

			mVertices[v + mVerticesNum].color.x = 1.0f;
			mVertices[v + mVerticesNum].color.y = 1.0f;
			mVertices[v + mVerticesNum].color.z = 1.0f;
			mVertices[v + mVerticesNum].color.w = 1.0f;

		}

		// �@���ǂݍ���
		FbxArray<FbxVector4> normal;
		mesh->GetPolygonVertexNormals(normal);
		for (int v = 0; v < normal.Size(); ++v)
		{
			mVertices[v + mVerticesNum].normal.x = (float)normal[v][0];
			mVertices[v + mVerticesNum].normal.y = (float)normal[v][1];
			mVertices[v + mVerticesNum].normal.z = (float)normal[v][2];
		}

		// UV�ǂݍ���
		FbxStringList names;
		mesh->GetUVSetNames(names);
		FbxArray<FbxVector2> uv;
		mesh->GetPolygonVertexUVs(names.GetStringAt(0), uv);
		for (int v = 0; v < uv.Size(); ++v)
		{
			mVertices[v + mVerticesNum].tex.x = (float)uv[v][0];
			mVertices[v + mVerticesNum].tex.y = (float)(1.0f - uv[v][1]);
		}

		// �ڐ�
		// �]�@��

		// �C���f�b�N�X�ݒ�
		for (int k = 0; k < num; k += 3)
		{
			mIndices[k + 0 + mVerticesNum] = k + 0 + mVerticesNum;
			mIndices[k + 1 + mVerticesNum] = k + 1 + mVerticesNum;
			mIndices[k + 2 + mVerticesNum] = k + 2 + mVerticesNum;
		}

		// �{�[���ǂݍ���
		LoadBone(mesh);

		
		// ���b�V���̎g�p�}�e���A���擾
		FbxLayerElementMaterial* lem = mesh->GetElementMaterial();
		if (lem != NULL)
		{
			// �|���S���ɓ\���Ă���}�e���A���ԍ�
			int materialIndex = lem->GetIndexArray().GetAt(0);

			// ���b�V���}�e���A����materialIndex�Ԗڂ��擾
			FbxSurfaceMaterial* material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(materialIndex);
			FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			FbxProperty factor = material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
			if (prop.IsValid() && factor.IsValid())
			{
				FbxDouble3 color = prop.Get<FbxDouble3>();
				double f = factor.Get<FbxDouble>();
				mMaterialColors[m].x = static_cast<float>(color[0] * f);
				mMaterialColors[m].y = static_cast<float>(color[1] * f);
				mMaterialColors[m].z = static_cast<float>(color[2] * f);
				mMaterialColors[m].w = 1.0f;
			}

			// �e�N�X�`���ǂݍ���
			const char* path = NULL;
			int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();
			if (fileTextureCount > 0)
			{
				FbxFileTexture* fileTex = prop.GetSrcObject<FbxFileTexture>(0);
				path = fileTex->GetFileName();
			}
			else
			{
				int numLayer = prop.GetSrcObjectCount<FbxLayeredTexture>();
				if (numLayer > 0)
				{
					FbxLayeredTexture* layerTex = prop.GetSrcObject<FbxLayeredTexture>(0);
					FbxFileTexture* fileTex = layerTex->GetSrcObject<FbxFileTexture>(0);
					path = fileTex->GetFileName();
				}
			}

			if (path != NULL)
			{
				// ���΃p�X�ɕϊ�
				size_t len = strnlen_s(path, STR_MAX);
				const char* imgName = &path[len];
				for (size_t i = 0; i < len; ++i)
				{
					imgName--;
					if (*imgName == '/') { ++imgName; break; }
					if (*imgName == '\\') { ++imgName; break; }
				}
				char relativeName[STR_MAX] = {};
				strcpy_s(relativeName, STR_MAX, mFbxDir); // fbx�̃p�X���R�s�[
				// strcat_s(relativeName, STR_MAX, "texture/"); // texture�t�H���_
				strcat_s(relativeName, STR_MAX, imgName); // png�̃t�@�C������A��

				// ���C�h�����ɕϊ�
				wchar_t filename[STR_MAX];
				setlocale(LC_ALL, "Japanese_Japan.932");
				size_t ret = 0;
				mbstowcs_s(&ret, mMaterials[m].filename, STR_MAX, imgName, _TRUNCATE);
				mbstowcs_s(&ret, filename, STR_MAX, relativeName, _TRUNCATE);

				// �e�N�X�`���ǂݍ���
				mMaterials[m].diffuse.Load(filename, D3D11_TEXTURE_ADDRESS_WRAP);

				// �m�[�}���}�b�v
				char normalMapName[STR_MAX] = {};
				strcpy_s(normalMapName, STR_MAX, mFbxDir); // fbx�̃p�X���R�s�[
				strcat_s(normalMapName, STR_MAX, "N"); // png�̃t�@�C������A��
				strcat_s(normalMapName, STR_MAX, imgName); // png�̃t�@�C������A��
				mbstowcs_s(&ret, filename, STR_MAX, normalMapName, _TRUNCATE);

				mMaterials[m].normal.Load(filename, D3D11_TEXTURE_ADDRESS_WRAP);
			}
			else
			{
				mMaterials[m].diffuse.Load(L"", D3D11_TEXTURE_ADDRESS_WRAP);
				mMaterials[m].normal.Load(L"", D3D11_TEXTURE_ADDRESS_WRAP);
			}




		}
		else
		{
			mMaterials[m].diffuse.Load(L"", D3D11_TEXTURE_ADDRESS_WRAP);
			mMaterials[m].normal.Load(L"", D3D11_TEXTURE_ADDRESS_WRAP);
		}

		mMaterials[m].faceNum = num / 3;

		mVerticesNum += num;
	}


	OptimizeVertices();

	// ���_���f�[�^�ۑ�
	mVerticesSource.resize(mVertices.size());
	for (size_t i = 0; i < mVerticesNum; ++i)
	{
		mVerticesSource[i].pos = mVertices[i].pos;
		mVerticesSource[i].normal = mVertices[i].normal;
	}

	// �ʏ��ۑ�
	mFaces.resize(mVerticesNum / 3);
	for (int i = 0; i < mVerticesNum / 3; ++i)
	{
		mFaces[i].vertex[0] = mVertices[mIndices[i * 3]].pos;
		mFaces[i].vertex[1] = mVertices[mIndices[i * 3 + 1]].pos;
		mFaces[i].vertex[2] = mVertices[mIndices[i * 3 + 2]].pos;
	}

	// �E�F�C�g���K��
	// 5�{�ȏ�ɂ܂������Ă�ꍇ�̂���
	for (int i = 0; i < mVerticesNum; ++i)
	{
		float n = 0;

		// ���_�̃E�F�C�g�̍��v�l
		for (int w = 0; w < mWeights[i].count; ++w)
		{
			n += mWeights[i].weight[w];
		}

		// ���K��
		for (int w = 0; w < mWeights[i].count; ++w)
		{
			mWeights[i].weight[w] /= n;
		}
	}

	scene->Destroy();
	manager->Destroy();

	Save(mBinFile);
}

void SkinnedMesh::LoadBone(FbxMesh* mesh)
{
	// ���b�V�����_��
	int num = mesh->GetPolygonVertexCount();

	// �X�L�����̗L��
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0)
	{
		return;
	}

	FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));

	// �{�[����
	int boneNum = skin->GetClusterCount();

	// �S�{�[�����擾
	for (int bone = 0; bone < boneNum; ++bone)
	{
		// �{�[�����擾
		FbxCluster* cluster = skin->GetCluster(bone);
		FbxAMatrix trans;
		cluster->GetTransformMatrix(trans);

		// �{�[����
		const char* boneName = cluster->GetLink()->GetName();

		// �{�[������
		bool isNewBone = false;
		int boneNo = FindBone(boneName);
		if (boneNo < 0)
		{
			boneNo = mBoneNum;
			++mBoneNum;
			isNewBone = true;
		}

		if (isNewBone)
		{
			strcpy_s(mBones[boneNo].name, STR_MAX, boneName);

			// �I�t�Z�b�g�s��쐬
			FbxAMatrix linkMatrix;
			cluster->GetTransformLinkMatrix(linkMatrix);

			FbxAMatrix offset = linkMatrix.Inverse() * trans;
			FbxDouble* offsetM = (FbxDouble*)offset;

			// �I�t�Z�b�g�s��ۑ�
			for (int i = 0; i < 16; ++i)
			{
				mBones[boneNo].offsetMatrix.m[i] = (float)offsetM[i];
			}

			// �L�[�t���[���ǂݍ���
			LoadKeyFrames(DEFAULT, boneNo, cluster->GetLink());
		}

		int weightCount = cluster->GetControlPointIndicesCount();
		int* weightIndex = cluster->GetControlPointIndices();
		double* weight = cluster->GetControlPointWeights();

		int* index = mesh->GetPolygonVertices();

		for (int i = 0; i < weightCount; ++i)
		{
			int weightIndex2 = weightIndex[i];

			// �S�|���S������weightIndex2�Ԗڂ̒��_����
			for (int v = 0; v < num; ++v)
			{
				if (index[v] != weightIndex2) continue;

				// ���_�ɃE�F�C�g��ۑ�
				int w = mWeights[v + mVerticesNum].count;
				if (w >= 4) continue;

				mWeights[v + mVerticesNum].bone[w] = boneNo;
				mWeights[v + mVerticesNum].weight[w] = (float)weight[i];
				mWeights[v + mVerticesNum].count++;
			}
		}
	}
}

int SkinnedMesh::FindBone(const char* name)
{
	int bone = -1; // ������Ȃ�
	for (int i = 0; i < mBoneNum; i++) {
		if (strcmp(name, mBones[i].name) == 0) {
			bone = i;
			break;
		}
	}
	return bone;
}

void SkinnedMesh::OptimizeVertices()
{
	int currentNum = 0;
	for (int v = 0; v < mVerticesNum; ++v)
	{
		int sameIndex = -1;
		for (int old = 0; old < currentNum; old++) 
		{
			if (mVertices[v].pos.x != mVertices[old].pos.x) continue;
			if (mVertices[v].pos.y != mVertices[old].pos.y) continue;
			if (mVertices[v].pos.z != mVertices[old].pos.z) continue;
			if (mVertices[v].normal.x != mVertices[old].normal.x) continue;
			if (mVertices[v].normal.y != mVertices[old].normal.y) continue;
			if (mVertices[v].normal.z != mVertices[old].normal.z) continue;
			if (mVertices[v].tex.x != mVertices[old].tex.x) continue;
			if (mVertices[v].tex.y != mVertices[old].tex.y) continue;

			sameIndex = old;
			break;
		}

		int target = v;
		if (sameIndex == -1)
		{
			CopyMemory(&mVertices[currentNum], &mVertices[v], sizeof(Vertex));
			CopyMemory(&mWeights[currentNum], &mWeights[v], sizeof(Weight));
			target = currentNum;
			++currentNum;
		}
		else
		{
			target = sameIndex;
		}

		for (int i = 0; i < mVerticesNum; ++i)
		{
			if (mIndices[i] == v) mIndices[i] = target;
		}
	}

	// �V�o�b�t�@�m��
	std::vector<Vertex> vertex;
	vertex.resize(currentNum);
	CopyMemory(vertex.data(), mVertices.data(), sizeof(Vertex) * currentNum);
	mVerticesNum = currentNum;

	mVertices.clear();
	mVertices = vertex;
}

void SkinnedMesh::LoadKeyFrames(MotionType type, int bone, FbxNode* boneNode)
{
	Motion& motion = mMotion[type];
	motion.keyframe[bone] = new Matrix[motion.frameNum + 1];

	double time = mStartFrame * (1.0 / 60);
	FbxTime t;
	for (int f = 0; f < mMotion[type].frameNum; ++f)
	{
		t.SetSecondDouble(time);

		// t�b�̎p���s���Get
		FbxMatrix m = boneNode->EvaluateGlobalTransform(t);

		FbxDouble* mat = (FbxDouble*)m;
		for (int i = 0; i < 16; ++i)
		{
			mMotion[type].keyframe[bone][f].m[i] = (float)mat[i];
			
		}

		time += 1.0 / 60.0;
	}
}


//	�{�[���s��̕��
void MatrixInterporate(Matrix& out, Matrix& A, Matrix& B, float rate)
{
	out = A * (1.0f - rate) + B * rate;
}


void SkinnedMesh::Skinning()
{
	Motion* motion = &mMotion[mMotionType];
	if (motion == NULL) return;

	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// �z��p�ϐ�
	int f = (int)mFrame;
	Matrix keyMatrix[BONE_MAX];

	// �s�񏀔�
	for (int b = 0; b < mBoneNum; ++b)
	{
		// �s��⊮
		Matrix mat;
		MatrixInterporate(mat, motion->keyframe[b][f], motion->keyframe[b][f + 1], mFrame - (int)mFrame);
		mBones[b].transform = mat;

		//�L�[�t���[��
		keyMatrix[b] = mBones[b].offsetMatrix * mat;
	}

	mSkinningShader->UpdateStructureBuffer(keyMatrix, 2);

	const float numThreadX = 256.0f;
	UINT groupX = static_cast<UINT>(ceil(mVerticesNum / numThreadX));
	mSkinningShader->Run(groupX, 1, 1); // ���_�� / �X���b�h��

	// �v�Z���ʂ�ǂݍ���
	{
		ID3D11Buffer* result = nullptr;
		mSkinningShader->CreateResultBuffer(&result);

		HRESULT hr = S_OK;

		D3D11_MAPPED_SUBRESOURCE ms;
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		hr = context->Map(result, 0, D3D11_MAP_READ, 0, &ms);
		if (FAILED(hr)) return;

		VertexForSkinning* v = static_cast<VertexForSkinning*>(ms.pData);
		for (int i = 0; i < mVerticesNum; ++i)
		{
			mVertices[i].pos    = v[i].pos;
			mVertices[i].normal = v[i].normal;
		}

		context->Unmap(result, 0);

		result->Release();
	}

	FRAMEWORK.GetContext()->UpdateSubresource(mVertexBuffer.Get(), 0, NULL, mVertices.data(), 0, 0);
	Animate(1.0f / 60.0f);

	//// ���_�ό`
	//for (int v = 0; v < mVerticesNum; ++v)
	//{
	//	// ���_ * �{�[���s��
	//	// b = v�Ԗڂ̒��_�̉e���{�[��
	//	if (mWeights[v].count <= 0)continue;
	//	mVertices[v].pos.x = 0;
	//	mVertices[v].pos.y = 0;
	//	mVertices[v].pos.z = 0;
	//	// �e���������[�v
	//	for (int n = 0; n < mWeights[v].count; ++n)
	//	{
	//		int b = mWeights[v].bone[n];
	//		float x = mVerticesSource[v].pos.x;
	//		float y = mVerticesSource[v].pos.y;
	//		float z = mVerticesSource[v].pos.z;
	//		// ���W���e���͕��ړ�
	//		mVertices[v].pos.x += (x * keyMatrix[b]._11 + y * keyMatrix[b]._21 + z * keyMatrix[b]._31 + 1 * keyMatrix[b]._41) * mWeights[v].weight[n];
	//		mVertices[v].pos.y += (x * keyMatrix[b]._12 + y * keyMatrix[b]._22 + z * keyMatrix[b]._32 + 1 * keyMatrix[b]._42) * mWeights[v].weight[n];
	//		mVertices[v].pos.z += (x * keyMatrix[b]._13 + y * keyMatrix[b]._23 + z * keyMatrix[b]._33 + 1 * keyMatrix[b]._43) * mWeights[v].weight[n];
	//		float nx = mVerticesSource[v].normal.x;
	//		float ny = mVerticesSource[v].normal.y;
	//		float nz = mVerticesSource[v].normal.z;
	//		//	�@�����e���͕��ϊ�
	//		mVertices[v].normal.x += (nx * keyMatrix[b]._11 + ny * keyMatrix[b]._21 + nz * keyMatrix[b]._31) * mWeights[v].weight[n];
	//		mVertices[v].normal.y += (nx * keyMatrix[b]._12 + ny * keyMatrix[b]._22 + nz * keyMatrix[b]._32) * mWeights[v].weight[n];
	//		mVertices[v].normal.z += (nx * keyMatrix[b]._13 + ny * keyMatrix[b]._23 + nz * keyMatrix[b]._33) * mWeights[v].weight[n];
	//	}
	//}
}

void SkinnedMesh::LoadMeshAnim(FbxMesh* mesh)
{
	FbxNode* node = mesh->GetNode();

	int boneNo = mBoneNum;
	strcpy_s(mBones[boneNo].name, STR_MAX, node->GetName());

	// �I�t�Z�b�g�s��
	mBones[boneNo].offsetMatrix.Identity();

	// �L�[�t���[���ǂݍ���
	LoadKeyFrames(DEFAULT, boneNo, node);

	// �E�F�C�g�ݒ�
	int num = mesh->GetPolygonVertexCount();
	for (int i = 0; i < num; ++i)
	{
		mWeights[i + mVerticesNum].bone[0] = boneNo;
		mWeights[i + mVerticesNum].weight[0] = 1.0f;
		mWeights[i + mVerticesNum].count = 1;
	}

	++mBoneNum;
}

bool SkinnedMesh::LoadBIN(const char* filename)
{
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "rb");
	if (!fp) return false;

	// ���_
	fread(&mVerticesNum, sizeof(int), 1, fp);
	mVertices.resize(mVerticesNum);
	fread(mVertices.data(), sizeof(Vertex), mVerticesNum, fp);

	// �����蔻��
	fread(&mAABB.min, sizeof(Vector3), 1, fp);
	fread(&mAABB.max, sizeof(Vector3), 1, fp);

	// �C���f�b�N�X
	size_t size = 0;
	fread(&size, sizeof(size_t), 1, fp);
	mIndices.resize(size);
	fread(mIndices.data(), sizeof(u_int), size, fp);

	// �ʏ��
	size = mVerticesNum / 3;
	mFaces.resize(size);
	fread(mFaces.data(), sizeof(Face), size, fp);

	// ���b�V����
	fread(&mMeshNum, sizeof(int), 1, fp);

	// �}�e���A���� <�t�@�C�����A�ʐ�>
	mMaterials.resize(mMeshNum);
	for (auto& material : mMaterials)
	{
		fread(material.filename, sizeof(wchar_t) * STR_MAX, 1, fp);
		fread(&material.faceNum, sizeof(int), 1, fp);

		// ���C�h�����ɕϊ�
		wchar_t filepath[STR_MAX];
		wchar_t nFilepath[STR_MAX];
		setlocale(LC_ALL, "Japanese_Japan.932");
		size_t ret = 0;
		mbstowcs_s(&ret, filepath, STR_MAX, mFbxDir, _TRUNCATE);
		wcscpy_s(nFilepath, STR_MAX, filepath);
		wcscat_s(nFilepath, STR_MAX, L"N");
		wcscat_s(filepath, STR_MAX, material.filename);
		wcscat_s(nFilepath, STR_MAX, material.filename);
		material.diffuse.Load(filepath, D3D11_TEXTURE_ADDRESS_WRAP);
		material.normal.Load(nFilepath, D3D11_TEXTURE_ADDRESS_WRAP);
	}

	// �}�e���A���J���[
	mMaterialColors.resize(mMeshNum);
	fread(mMaterialColors.data(), sizeof(DirectX::XMFLOAT4), mMaterialColors.size(), fp);

	// �{�[��
	fread(&mBoneNum, sizeof(int), 1, fp);
	fread(mBones, sizeof(Bone), mBoneNum, fp);

	// �E�F�C�g
	mWeights.resize(mVerticesNum);
	fread(mWeights.data(), sizeof(Weight), mVerticesNum, fp);

	// ���_���f�[�^
	mVerticesSource.resize(mVerticesNum);
	for (size_t i = 0; i < mVerticesNum; ++i)
	{
		mVerticesSource[i].pos = mVertices[i].pos;
		mVerticesSource[i].normal = mVertices[i].normal;
	}

	// ���[�V����
	size = 0;
	fread(&size, sizeof(size_t), 1, fp);
	for (size_t i = 0; i < size; ++i)
	{
		MotionType type;
		fread(&type, sizeof(int), 1, fp);


		auto& motion = mMotion[type];
		fread(&motion.frameNum, sizeof(int), 1, fp);
		for (int b = 0; b < mBoneNum; ++b)
		{
			motion.keyframe[b] = new Matrix[motion.frameNum + 1];
			fread(motion.keyframe[b], sizeof(Matrix), motion.frameNum + 1, fp);
		}
	}

	fclose(fp);
	return true;
}

void SkinnedMesh::Save(const char* filename)
{
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "wb");

	// ���_
	fwrite(&mVerticesNum, sizeof(int), 1, fp);
	fwrite(mVertices.data(), sizeof(Vertex), mVerticesNum, fp);

	// �����蔻��
	fwrite(&mAABB.min, sizeof(Vector3), 1, fp);
	fwrite(&mAABB.max, sizeof(Vector3), 1, fp);

	// �C���f�b�N�X
	size_t size = mIndices.size();
	fwrite(&size, sizeof(size_t), 1, fp);
	fwrite(mIndices.data(), sizeof(u_int), size, fp);

	// �ʏ��(���� ���_�� / 3)
	fwrite(mFaces.data(), sizeof(Face), mFaces.size(), fp);

	// ���b�V���� 
	fwrite(&mMeshNum, sizeof(int), 1, fp);

	// �}�e���A���� <�t�@�C�����A�ʐ�>
	for (auto& material : mMaterials)
	{
		fwrite(material.filename, sizeof(wchar_t) * STR_MAX, 1, fp);
		fwrite(&material.faceNum, sizeof(int), 1, fp);
	}

	// �}�e���A���J���[
	fwrite(mMaterialColors.data(), sizeof(DirectX::XMFLOAT4), mMaterialColors.size(), fp);

	// �{�[��
	fwrite(&mBoneNum, sizeof(int), 1, fp);
	fwrite(mBones, sizeof(Bone), mBoneNum, fp);

	// �E�F�C�g�ۑ�
	fwrite(mWeights.data(), sizeof(Weight), mVerticesNum, fp);

	// ���[�V�����ۑ�
	size_t motionNum = mMotion.size();
	fwrite(&motionNum, sizeof(size_t), 1, fp);
	for (auto& motion : mMotion)
	{
		// key1(MotionType)�ۑ� 
		fwrite(&motion.first, sizeof(int), 1, fp);

		// key2(Motion)�ۑ�
		fwrite(&motion.second.frameNum, sizeof(int), 1, fp);
		for (int i = 0; i < mBoneNum; ++i)
			fwrite(motion.second.keyframe[i], sizeof(Matrix), motion.second.frameNum + 1, fp);
	}


	fclose(fp);
}


SkinnedMesh::SkinnedMesh(const char* fbxFilename)
{
	mConstBuffer = nullptr;
	mAABB.min = Vector3( 0.0f, 0.0f, 0.0f );
	mAABB.max = Vector3( 0.0f, 0.0f, 0.0f );

	Initialize(fbxFilename);
}

SkinnedMesh::~SkinnedMesh()
{
	mShader.UnLoad();
	for (auto& m : mMaterials)
	{
		m.diffuse.UnLoad();
		m.normal.UnLoad();
	}
	mVertices.clear();
	mVerticesSource.clear();
	mIndices.clear();
	mMaterials.clear();
	mMaterialColors.clear();
	mWeights.clear();


	// ���[�V�����֘A���
	for (auto& m : mMotion)
	{
		for (int bone = 0; bone < mBoneNum; ++bone)
		{
			delete[] m.second.keyframe[bone];
			m.second.keyframe[bone] = nullptr;
		}
	}
	//mMotion.clear();
}

void SkinnedMesh::Initialize(const char* fbxFilename)
{
	auto device = FRAMEWORK.GetDevice();

	strcpy_s(mFbxDir, 128, fbxFilename);// AA/BB/abc.fbx
	for (int i = strlen(mFbxDir) - 1; i >= 0; --i)
	{
		if (mFbxDir[i] == '/' || mFbxDir[i] == '\\')
		{
			mFbxDir[i + 1] = '\0'; // AA/BB/
			break;
		}
	}

	LoadFBX(device, fbxFilename);

	HRESULT hr;

	// D3D11_APPEND_ALIGNED_ELEMENT �͑O�̃f�[�^�̒���ɍ���Ă�����炵��(sizeof�Ƃ�����Ȃ��̂ł�)
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	if (mBoneNum > 0)
	{
		mSkinningShader = std::make_unique<ComputeShader>("Shaders/ComputeShaders/Skinning/Skinning_cs.cso");
		mSkinningShader->CreateStructuredBuffer(mVerticesSource.data(), mVerticesSource.size(), sizeof(VertexForSkinning));
		mSkinningShader->CreateStructuredBuffer(mWeights.data(), mWeights.size(), sizeof(Weight));
		mSkinningShader->CreateStructuredBuffer(nullptr, mBoneNum, sizeof(Matrix));
		mSkinningShader->CreateRWStructuredBuffer(mVerticesSource.size(), sizeof(VertexForSkinning));
		mSkinningShader->CreateViews();
	}


	mShader.LoadVS("Shaders/SkinnedMesh/SkinnedMesh_vs.cso", layout, numElements);
	mShader.LoadPS("Shaders/SkinnedMesh/SkinnedMesh_ps.cso");

	D3D11_SUBRESOURCE_DATA subresourceData;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&subresourceData, sizeof(subresourceData));
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	subresourceData.pSysMem = mVertices.data();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * mVertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bufferDesc, &subresourceData, mVertexBuffer.GetAddressOf());
	if (FAILED(hr)) return;


	subresourceData.pSysMem = mIndices.data();
	bufferDesc.ByteWidth = sizeof(u_int) * mIndices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bufferDesc, &subresourceData, mIndexBuffer.GetAddressOf());
	if (FAILED(hr)) return;


	bufferDesc.ByteWidth = sizeof(Cbuffer);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bufferDesc, NULL, mConstBuffer.GetAddressOf());
	if (FAILED(hr)) return;
}

void SkinnedMesh::Render(const DirectX::XMFLOAT4X4& wvp, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& lightDirection, float elapsedTime, float alpha)
{
	// ���[�V����������Ȃ�skinning
	if (mMotion[mMotionType].frameNum > 0)
	{
		//Skinning();
	}
	const auto& context = FRAMEWORK.GetContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	// ���X�^���C�U / �[�x�X�e���V���擾
	ID3D11RasterizerState* rs = FRAMEWORK.GetRasterizer(Framework::RS_CULL_BACK);
	if(mIsSwapYZ) rs = FRAMEWORK.GetRasterizer(Framework::RS_SWAPYZ);
	ID3D11DepthStencilState* ds = FRAMEWORK.GetDepthStencil(Framework::DS_TRUE);

	mShader.Activate();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	context->RSSetState(rs);
	context->OMSetDepthStencilState(ds, 0);

	int start = 0;
	for (int m = 0; m < mMeshNum; ++m)
	{
		mMaterials[m].diffuse.Set(0);
		mMaterials[m].normal.Set(1);

		Cbuffer cb;
		DirectX::XMStoreFloat4x4(&cb.wvp,  DirectX::XMLoadFloat4x4(&coordConversion) * DirectX::XMLoadFloat4x4(&wvp));
		DirectX::XMStoreFloat4x4(&cb.world, DirectX::XMLoadFloat4x4(&coordConversion) * DirectX::XMLoadFloat4x4(&world));
		cb.lightDirection = lightDirection;
		cb.materialColor = mMaterialColors[m];
		context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, &cb, 0, 0);
		context->VSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());

		// �`��
		context->DrawIndexed(mMaterials[m].faceNum * 3, start, 0);
		start += mMaterials[m].faceNum * 3;
	}
}

int SkinnedMesh::RayPick(const DirectX::XMFLOAT3& sp, const DirectX::XMFLOAT3& ep, DirectX::XMFLOAT3* outPos, DirectX::XMFLOAT3* outNormal, float* outLen)
{
	int ret = -1;
	DirectX::XMVECTOR start = DirectX::XMLoadFloat3(&sp);
	DirectX::XMVECTOR end = DirectX::XMLoadFloat3(&ep);
	DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);
	DirectX::XMVECTOR length = DirectX::XMVector3Length(vec);
	DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(vec);
	float neart;
	DirectX::XMStoreFloat(&neart, length);

	// �������̂���
	DirectX::XMVECTOR vertex[3] = {};
	DirectX::XMVECTOR nor = {};
	DirectX::XMVECTOR dot = {};
	DirectX::XMVECTOR v0 = {};
	DirectX::XMVECTOR d1 = {};
	DirectX::XMVECTOR t = {};
	DirectX::XMVECTOR cp = {};
	DirectX::XMVECTOR tempV = {};
	DirectX::XMVECTOR tempN = {};
	DirectX::XMVECTOR position = {}, normal = {};
	for (const auto& it : mFaces)
	{
		// �ʒ��_�擾

		vertex[0] = DirectX::XMLoadFloat3(&it.vertex[0]); // a
		vertex[1] = DirectX::XMLoadFloat3(&it.vertex[1]); // b 
		vertex[2] = DirectX::XMLoadFloat3(&it.vertex[2]); // c

		// 3�ӎZ�o
		DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(vertex[1], vertex[0]);
		DirectX::XMVECTOR bc = DirectX::XMVectorSubtract(vertex[2], vertex[1]);
		DirectX::XMVECTOR ca = DirectX::XMVectorSubtract(vertex[0], vertex[2]);

		// �O�ςɂ��@���Z�o
		nor = DirectX::XMVector3Cross(ab, bc);
		nor = DirectX::XMVector3Normalize(nor);

		// ���ς̌��ʂ��v���X�Ȃ�Η�����
		dot = DirectX::XMVector3Dot(nor, dir);
		if (DirectX::XMVectorGetX(dot) > 0) continue;

		// ��_�Z�o
		v0 = DirectX::XMVectorSubtract(vertex[0], start); // �|���S����1���_���烌�C�̊J�n�n�_�̃x�N�g��
		d1 = DirectX::XMVector3Dot(v0, nor);				// ���̃x�N�g���ƃ|���S���̖@���̓���(�ˉe���o��)
		t = DirectX::XMVectorDivide(d1, dot);				//  ���̃x�N�g���Ɩ@���̊���������
		float ft = 0.0f;
		DirectX::XMStoreFloat(&ft, t);
		if (ft < 0.0f || ft > neart) continue;

		// ��_
		cp = DirectX::XMVectorAdd(start, DirectX::XMVectorMultiply(dir, t));

		// ���_����
		// 1��
		tempV = DirectX::XMVectorSubtract(vertex[0], cp); //pa
		tempN = DirectX::XMVector3Cross(tempV, ab);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // ���ς̌��ʃ}�C�i�X�Ȃ�O��

		// 2��
		tempV = DirectX::XMVectorSubtract(vertex[1], cp); //pb
		tempN = DirectX::XMVector3Cross(tempV, bc);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // ���ς̌��ʃ}�C�i�X�Ȃ�O��

		// 3��
		tempV = DirectX::XMVectorSubtract(vertex[2], cp); //pc
		tempN = DirectX::XMVector3Cross(tempV, ca);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // ���ς̌��ʃ}�C�i�X�Ȃ�O��

		// ���ۑ�
		position = cp;
		normal = nor;
		neart = ft;
		ret = it.materialIndex;
	}
	if (ret != -1)
	{
		// ��������
		DirectX::XMStoreFloat3(outPos, position);
		DirectX::XMStoreFloat3(outNormal, normal);
	}

	// �ł��߂��q�b�g�ʒu�܂ł̋���
	*outLen = neart;
	return ret;


}

void SkinnedMesh::SetMotion(MotionType type, bool isLoop)
{
	if (mMotionType == type) return;
	mMotionType = type;
	mIsLoop = isLoop;
	mFrame = 0.0f;
}

void SkinnedMesh::AddMotion(const char* filename, MotionType type)
{
	// ���łɃL�[���o�^����Ă���return
	if (mMotion.count(type) >= 1) return;

	FbxManager* manager = FbxManager::Create();
	FbxScene* scene = FbxScene::Create(manager, "");

	// �t�@�C������V�[���ɓǂݍ���
	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename);
	importer->Import(scene);
	importer->Destroy();

	// ���[�V�������擾
	FbxArray<FbxString*> names;
	scene->FillAnimStackNameArray(names);

	FbxTakeInfo* take = scene->GetTakeInfo(names[0]->Buffer());
	FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
	FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
	FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	mStartFrame = (int)(start / fps60);
	mMotion[type].frameNum = (int)((stop - start) / fps60);
	
	FbxNode* root = scene->GetRootNode();

	// �S�{�[���ǂݍ���
	for (int b = 0; b < mBoneNum; ++b)
	{
		FbxNode* bone = root->FindChild(mBones[b].name);
		if (bone == NULL) continue;

		LoadKeyFrames(type, b, bone);
	}


	int size = names.Size();
	for (int i = 0; i < size; ++i)
	{
		delete names[i];
	}
	scene->Destroy();
	manager->Destroy();

	// �X�V
	Save(mBinFile);
}

void SkinnedMesh::Animate(float elapsedTime)
{
	// ���[�V�������Ԃ̍X�V
	mFrame += elapsedTime * 60;

	// ���[�v�`�F�b�N
	if (mFrame >= mMotion[mMotionType].frameNum - 1)
	{
		mFrame = 0;

		if (!mIsLoop) SetMotion(IDLE, true); 
	}
}


