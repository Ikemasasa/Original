#include "SkinnedMesh.h"

#include "Framework.h"

#include "ConvertString.h"
#include "Font.h"
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
	//geometryConverter.SplitMeshesPerMaterial(scene, true);
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
	mVertices = new Vertex[vertexNum];
	mIndices = new u_int[vertexNum];
	mWeights = new Weight[vertexNum];

	// �}�e���A�����Ƃ̃|���S�����_��
	mMaterials = new Material[mMeshNum];
	mMaterialColors = new Vector4[mMeshNum];

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
		{
			if (mesh->GetElementTangentCount() > 0)
			{
				FbxGeometryElementTangent* element = mesh->GetElementTangent();// mesh->CreateElementTangent();
				//FbxLayerElement::EMappingMode mapmode = element->GetMappingMode();
				//FbxLayerElement::EReferenceMode refmode = element->GetReferenceMode();

				FbxLayerElementArrayTemplate<FbxVector4>* index = &element->GetDirectArray();
				int indexCount = index->GetCount();
				for (int j = 0; j < indexCount; j++) {
					// FbxColor�擾
					FbxVector4 v = index->GetAt(j);
					// DWORD�^�̃J���[�쐬        
					mVertices[j + mVerticesNum].tangent.x = (float)v[0];
					mVertices[j + mVerticesNum].tangent.y = (float)v[1];
					mVertices[j + mVerticesNum].tangent.z = (float)v[2];
				}
			}
			


		}

		// �]�@��
		{
			if (mesh->GetElementBinormalCount() > 0)
			{
				FbxGeometryElementBinormal* element = mesh->GetElementBinormal();// mesh->CreateElementTangent();
				//FbxLayerElement::EMappingMode mapmode = element->GetMappingMode();
				//FbxLayerElement::EReferenceMode refmode = element->GetReferenceMode();

				FbxLayerElementArrayTemplate<FbxVector4>* index = &element->GetDirectArray();
				int indexCount = index->GetCount();
				for (int j = 0; j < indexCount; j++) {
					// FbxColor�擾
					FbxVector4 v = index->GetAt(j);
					// DWORD�^�̃J���[�쐬        
					mVertices[j + mVerticesNum].binormal.x = (float)v[0];
					mVertices[j + mVerticesNum].binormal.y = (float)v[1];
					mVertices[j + mVerticesNum].binormal.z = (float)v[2];
				}
			}

		}

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
			auto LoadTexture = [](FbxProperty* prop, MaterialSprite* spr, char fbxDir[STR_MAX])
			{
				std::string path; // ��΃p�X�擾�p

				int fileTextureCount = prop->GetSrcObjectCount<FbxFileTexture>();
				if (fileTextureCount > 0)
				{
					FbxFileTexture* fileTex = prop->GetSrcObject<FbxFileTexture>(0);
					path = fileTex->GetFileName(); // ��΃p�X
				}
				else
				{
					int numLayer = prop->GetSrcObjectCount<FbxLayeredTexture>();
					if (numLayer > 0)
					{
						FbxLayeredTexture* layerTex = prop->GetSrcObject<FbxLayeredTexture>(0);
						FbxFileTexture* fileTex = layerTex->GetSrcObject<FbxFileTexture>(0);
						path = fileTex->GetFileName();
					}
				}

				// �󂶂�Ȃ��Ȃ�
				if (!path.empty())
				{
					// ��΃p�X����e�N�X�`���t�@�C�����𔲂�
					size_t len = path.size() - 1;
					const char* imgName = &path[len];
					for (size_t i = 0; i < len; ++i)
					{
						imgName--;
						if (*imgName == '/') { ++imgName; break; }
						if (*imgName == '\\') { ++imgName; break; }
					}

					// ���΃p�X�쐬
					std::string work = fbxDir; // AA/BB/
					work += imgName;		   // AA/BB/*.fbx
					std::wstring relative = ConvertString::ConvertToWstirng(work);

					// �ǂݍ���
					spr->sprite.Load(relative.c_str());
					wcscpy_s(spr->filename, STR_MAX, relative.c_str());
				}
				else
				{
					// �󂾂�����_�~�[�e�N�X�`����ǂނ悤�ɂ���
					const wchar_t* dummy = L"";
					spr->sprite.Load(dummy);
					wcscpy_s(spr->filename, STR_MAX, dummy);
				}
			};

			if (prop.IsValid()) LoadTexture(&prop, &mMaterials[m].mtlSpr[0], mFbxDir); // Diffuse

			prop = material->FindProperty(FbxSurfaceMaterial::sNormalMap);
			if (prop.IsValid()) LoadTexture(&prop, &mMaterials[m].mtlSpr[1], mFbxDir); // NormalMap

			//const char* path = NULL;
			//int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();
			//if (fileTextureCount > 0)
			//{
			//	FbxFileTexture* fileTex = prop.GetSrcObject<FbxFileTexture>(0);
			//	path = fileTex->GetFileName();
			//}
			//else
			//{
			//	int numLayer = prop.GetSrcObjectCount<FbxLayeredTexture>();
			//	if (numLayer > 0)
			//	{
			//		FbxLayeredTexture* layerTex = prop.GetSrcObject<FbxLayeredTexture>(0);
			//		FbxFileTexture* fileTex = layerTex->GetSrcObject<FbxFileTexture>(0);
			//		path = fileTex->GetFileName();
			//	}
			//}

			//if (path != NULL)
			//{
			//	// ���΃p�X�ɕϊ�
			//	size_t len = strnlen_s(path, STR_MAX);
			//	const char* imgName = &path[len];
			//	for (size_t i = 0; i < len; ++i)
			//	{
			//		imgName--;
			//		if (*imgName == '/') { ++imgName; break; }
			//		if (*imgName == '\\') { ++imgName; break; }
			//	}
			//	char relativeName[STR_MAX] = {};
			//	strcpy_s(relativeName, STR_MAX, mFbxDir); // fbx�̃p�X���R�s�[
			//	// strcat_s(relativeName, STR_MAX, "texture/"); // texture�t�H���_
			//	strcat_s(relativeName, STR_MAX, imgName); // png�̃t�@�C������A��

			//	// ���C�h�����ɕϊ�
			//	wchar_t filename[STR_MAX];
			//	setlocale(LC_ALL, "Japanese_Japan.932");
			//	size_t ret = 0;
			//	mbstowcs_s(&ret, mMaterials[m].filename, STR_MAX, imgName, _TRUNCATE);
			//	mbstowcs_s(&ret, filename, STR_MAX, relativeName, _TRUNCATE);

			//	// �e�N�X�`���ǂݍ���
			//	mMaterials[m].diffuse.Load(filename);

			//	// �m�[�}���}�b�v
			//	char normalMapName[STR_MAX] = {};
			//	strcpy_s(normalMapName, STR_MAX, mFbxDir); // fbx�̃p�X���R�s�[
			//	strcat_s(normalMapName, STR_MAX, "N"); // png�̃t�@�C������A��
			//	strcat_s(normalMapName, STR_MAX, imgName); // png�̃t�@�C������A��
			//	mbstowcs_s(&ret, filename, STR_MAX, normalMapName, _TRUNCATE);

			//	mMaterials[m].normal.Load(filename);
			//}
			//else
			//{
			//	mMaterials[m].diffuse.Load(L"");
			//	mMaterials[m].normal.Load(L"");
			//}

		//}
		//else
		//{
		//	mMaterials[m].diffuse.Load(L"");
		//	mMaterials[m].normal.Load(L"");
		//}
		}
		mMaterials[m].faceNum = num / 3;

		mVerticesNum += num;
	}

	mFaceNum = mVerticesNum / 3;
	OptimizeVertices();

	// ���_���f�[�^�ۑ�
	mVerticesSource = new VertexForSkinning[mVerticesNum];
	for (int i = 0; i < mVerticesNum; ++i)
	{
		mVerticesSource[i].pos = mVertices[i].pos;
		mVerticesSource[i].normal = mVertices[i].normal;
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
	Vertex* vertex = new Vertex[currentNum];
	CopyMemory(vertex, mVertices, sizeof(Vertex) * currentNum);
	mVerticesNum = currentNum;

	delete[] mVertices;
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

void SkinnedMesh::Skinning()
{
	Motion* motion = &mMotion[mMotionType];
	if (motion == NULL) return;

	ID3D11DeviceContext* context = FRAMEWORK.GetContext();

	// �z��p�ϐ�
	int f = (int)mFrame;
	int nf = f + 1; // nextFrame
	if (f == motion->frameNum - 1) // f���Ō�̃t���[���Ȃ�
	{
		nf = 0;
	}

	Matrix keyMatrix[BONE_MAX];

	auto MatrixInterporate = [](Matrix& out, const Matrix& A, const Matrix& B, float rate)
	{
		//	�{�[���s��̕��
		out = A * (1.0f - rate) + B * rate;
	};

	// �s�񏀔�
	for (int b = 0; b < mBoneNum; ++b)
	{
		// �s��⊮
		Matrix mat;
		MatrixInterporate(mat, motion->keyframe[b][f], motion->keyframe[b][nf], mFrame - f);
		mBones[b].transform = mat;

		//�L�[�t���[��
		keyMatrix[b] = mBones[b].offsetMatrix * mat;
	}

	mSkinningShader->UpdateStructureBuffer(keyMatrix, 2);

	CbufferForSkinning cb;
	{
		//�萔�o�b�t�@�X�V
		const float numThreadX = 256.0f;
		cb.groupNumX = static_cast<UINT>(ceil(mVerticesNum / numThreadX));
		cb.groupNumY = static_cast<UINT>(1);
		cb.groupNumZ = static_cast<UINT>(1);
		cb.verticesNum = mVerticesNum;
		mSkinningShader->UpdateConstantBuffer(&cb);
	}
	
	mSkinningShader->Run(cb.groupNumX, cb.groupNumY, cb.groupNumZ);

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

	FRAMEWORK.GetContext()->UpdateSubresource(mVertexBuffer.Get(), 0, NULL, mVertices, 0, 0);
	Animate(1.0f / 60.0f);
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

	// ���_��
	fread(&mVerticesNum, sizeof(int), 1, fp);

	// ���_�ǂݍ���
	mVertices = new Vertex[mVerticesNum];
	fread(mVertices, sizeof(Vertex), mVerticesNum, fp);

	// AABB
	fread(&mAABB.min, sizeof(Vector3), 1, fp);
	fread(&mAABB.max, sizeof(Vector3), 1, fp);
	
	// �ʐ�
	fread(&mFaceNum, sizeof(int), 1, fp);

	// �C���f�b�N�X
	mIndices = new u_int[mFaceNum * 3];
	fread(mIndices, sizeof(u_int), mFaceNum * 3, fp);

	// ���b�V����
	fread(&mMeshNum, sizeof(int), 1, fp);

	// �}�e���A���� <�t�@�C�����A�ʐ�>
	mMaterials = new Material[mMeshNum];
	for (int m = 0; m < mMeshNum; ++m)
	{
		for (int i = 0; i < Material::KIND; ++i)
		{
			fread(mMaterials[m].mtlSpr[i].filename, sizeof(wchar_t) * STR_MAX, 1, fp);
			mMaterials[m].mtlSpr[i].sprite.Load(mMaterials[m].mtlSpr[i].filename); // �摜�ǂݍ���
		}

		fread(&mMaterials[m].faceNum, sizeof(int), 1, fp);
	}
	//for (int i = 0; i < mMeshNum ;++i)
	//{
	//	fread(mMaterials[i].filename, sizeof(wchar_t) * STR_MAX, 1, fp);
	//	fread(&mMaterials[i].faceNum, sizeof(int), 1, fp);

	//	// ���C�h�����ɕϊ�
	//	wchar_t filepath[STR_MAX];
	//	wchar_t nFilepath[STR_MAX];
	//	setlocale(LC_ALL, "Japanese_Japan.932");
	//	size_t ret = 0;
	//	mbstowcs_s(&ret, filepath, STR_MAX, mFbxDir, _TRUNCATE);
	//	wcscpy_s(nFilepath, STR_MAX, filepath);
	//	wcscat_s(nFilepath, STR_MAX, L"N");
	//	wcscat_s(filepath, STR_MAX, mMaterials[i].filename);
	//	wcscat_s(nFilepath, STR_MAX, mMaterials[i].filename);
	//	mMaterials[i].diffuse.Load(filepath);
	//	mMaterials[i].normal.Load(nFilepath);
	//}

	// �}�e���A���J���[
	mMaterialColors = new Vector4[mMeshNum];
	fread(mMaterialColors, sizeof(DirectX::XMFLOAT4), mMeshNum, fp);

	// �{�[��
	fread(&mBoneNum, sizeof(int), 1, fp);
	fread(mBones, sizeof(Bone), mBoneNum, fp);

	// �E�F�C�g
	mWeights = new Weight[mVerticesNum];
	fread(mWeights, sizeof(Weight), mVerticesNum, fp);

	// ���_���f�[�^
	mVerticesSource = new VertexForSkinning[mVerticesNum];
	for (int i = 0; i < mVerticesNum; ++i)
	{
		mVerticesSource[i].pos = mVertices[i].pos;
		mVerticesSource[i].normal = mVertices[i].normal;
	}

	// ���[�V����
	size_t motionNum = 0;
	fread(&motionNum, sizeof(size_t), 1, fp);
	for (size_t i = 0; i < motionNum; ++i)
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
	fwrite(mVertices, sizeof(Vertex), mVerticesNum, fp);

	// AABB
	fwrite(&mAABB.min, sizeof(Vector3), 1, fp);
	fwrite(&mAABB.max, sizeof(Vector3), 1, fp);

	// �C���f�b�N�X
	fwrite(&mFaceNum, sizeof(int), 1, fp);
	fwrite(mIndices, sizeof(u_int), mFaceNum * 3, fp);

	// ���b�V���� 
	fwrite(&mMeshNum, sizeof(int), 1, fp);

	// �}�e���A���� <�t�@�C�����A�ʐ�>�A
	for (int m = 0; m < mMeshNum; ++m)
	{
		for (int i = 0; i < Material::KIND; ++i)
		{
			fwrite(mMaterials[m].mtlSpr[i].filename, sizeof(wchar_t) * STR_MAX, 1, fp);
		}

		fwrite(&mMaterials[m].faceNum, sizeof(int), 1, fp);
	}

	//for (int i = 0; i < mMeshNum; ++i)
	//{
	//	fwrite(mMaterials[i].filename, sizeof(wchar_t) * STR_MAX, 1, fp);
	//	fwrite(&mMaterials[i].faceNum, sizeof(int), 1, fp);
	//}
	// �}�e���A���J���[
	fwrite(mMaterialColors, sizeof(Vector4), mMeshNum, fp);

	// �{�[��
	fwrite(&mBoneNum, sizeof(int), 1, fp);
	fwrite(mBones, sizeof(Bone), mBoneNum, fp);

	// �E�F�C�g�ۑ�
	fwrite(mWeights, sizeof(Weight), mVerticesNum, fp);

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

	mShader = std::make_unique<Shader>();

	Initialize(fbxFilename);
}

SkinnedMesh::~SkinnedMesh()
{
	mShader->UnLoad();
	for (int i = 0; i < mMeshNum; ++i)
	{
		for (int k = 0; k < Material::KIND; ++k)
		{
			mMaterials[i].mtlSpr[k].sprite.UnLoad();
		}
	}
	delete[] mVertices;
	delete[] mVerticesSource;
	delete[] mIndices;
	delete[] mMaterials;
	delete[] mMaterialColors;
	delete[] mWeights;


	// ���[�V�����֘A���
	for (auto& m : mMotion)
	{
		for (int bone = 0; bone < mBoneNum; ++bone)
		{
			delete[] m.second.keyframe[bone];
			m.second.keyframe[bone] = nullptr;
		}
	}
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

	if (mBoneNum > 0)
	{
		mSkinningShader = std::make_unique<ComputeShader>(L"Shaders/ComputeShaders/Skinning.fx", "main");
		mSkinningShader->CreateStructuredBuffer(mVerticesSource, mVerticesNum, sizeof(VertexForSkinning));
		mSkinningShader->CreateStructuredBuffer(mWeights, mVerticesNum, sizeof(Weight));
		mSkinningShader->CreateStructuredBuffer(nullptr, mBoneNum, sizeof(Matrix));
		mSkinningShader->CreateRWStructuredBuffer(mVerticesNum, sizeof(VertexForSkinning));
		//mSkinningShader->CreateRWByteaddressBuffer(mVertices, mVerticesNum, sizeof(Vertex));
		mSkinningShader->CreateConstantBuffer(sizeof(CbufferForSkinning));

		//mVertexBuffer = mSkinningShader->GetRWBuffer();
	}


	mShader->Load(L"Shaders/SkinnedMesh.fx", "VSMain", "PSMain");


	D3D11_SUBRESOURCE_DATA subresourceData;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&subresourceData, sizeof(subresourceData));
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	if (!mVertexBuffer)
	{
		subresourceData.pSysMem = mVertices;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(Vertex) * mVerticesNum;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		hr = device->CreateBuffer(&bufferDesc, &subresourceData, mVertexBuffer.GetAddressOf());
		if (FAILED(hr)) return;
	}


	subresourceData.pSysMem = mIndices;
	bufferDesc.ByteWidth = sizeof(u_int) * mFaceNum * 3;
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
	const auto& context = FRAMEWORK.GetContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	mShader->Activate();

	context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	int start = 0;
	for (int m = 0; m < mMeshNum; ++m)
	{
		for (int i = 0; i < Material::KIND; ++i)
		{
			mMaterials[m].mtlSpr[i].sprite.Set(i);
		}

		Cbuffer cb;
		DirectX::XMStoreFloat4x4(&cb.wvp,  DirectX::XMLoadFloat4x4(&wvp));
		DirectX::XMStoreFloat4x4(&cb.world, DirectX::XMLoadFloat4x4(&world));
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

void SkinnedMesh::Render(const Shader* shader, const DirectX::XMFLOAT4X4& wvp, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& lightDirection, float elapsedTime, float alpha)
{
	const auto& context = FRAMEWORK.GetContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->Activate();

	int start = 0;
	for (int m = 0; m < mMeshNum; ++m)
	{
		for (int i = 0; i < Material::KIND; ++i)
		{
			mMaterials[m].mtlSpr[i].sprite.Set(i);
		}
		Cbuffer cb;
		DirectX::XMStoreFloat4x4(&cb.wvp, DirectX::XMLoadFloat4x4(&wvp));
		DirectX::XMStoreFloat4x4(&cb.world, DirectX::XMLoadFloat4x4(&world));
		cb.lightDirection = lightDirection;
		cb.materialColor = mMaterialColors[m];
		context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, &cb, 0, 0);
		context->VSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());

		// �`��
		context->DrawIndexed(mMaterials[m].faceNum * 3, start, 0);
		start += mMaterials[m].faceNum * 3;
	}

	// CS��VB���g���Ă�̂Ń��Z�b�g
	stride = 0;
	ID3D11Buffer* dummyVB = nullptr;
	context->IASetVertexBuffers(0, 1, &dummyVB, &stride, &offset);
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
	for (int i = 0; i < mFaceNum; ++i)
	{

		// �ʒ��_�擾
		int index0 = mIndices[i * 3 + 0];
		vertex[0] = DirectX::XMLoadFloat3(&mVertices[index0].pos); // a

		int index1 = mIndices[i * 3 + 1];
		vertex[1] = DirectX::XMLoadFloat3(&mVertices[index1].pos); // b 

		int index2 = mIndices[i * 3 + 2];
		vertex[2] = DirectX::XMLoadFloat3(&mVertices[index2].pos); // c

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
		ret = i; // i�Ԗڂ̃|���S���Ƀq�b�g
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

void SkinnedMesh::ChangeShader(Shader* shader)
{
	if (!shader) return;

	mShader->UnLoad();
	mShader.reset(shader);
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
	mMotionFinished = false;

	auto& nowMotion = mMotion[mMotionType];
	int f = static_cast<int>(mFrame);


	// �R���W�����`�F�b�N
	if (nowMotion.colBeginFrame != nowMotion.colEndFrame)
	{
		if (f >= nowMotion.colBeginFrame) nowMotion.isCollisionEnable = true;
		if (f <  nowMotion.colEndFrame) nowMotion.isCollisionEnable = false;
	}

	// ���[�v�`�F�b�N
	if (f >= nowMotion.frameNum - 1)
	{
		mFrame = 0;

		if (!mIsLoop)
		{
			SetMotion(IDLE, true);
			mMotionFinished = true;
		}
	}
}


