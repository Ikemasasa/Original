#include "SkinnedMesh.h"

#include "Framework.h"
#include "Math.h"
#include "ResourceManager.h"

#include <chrono>
#include "Font.h"

using namespace fbxsdk;

void SkinnedMesh::LoadFBX(ID3D11Device* device, const char* filename)
{	
	// 最適化ファイルチェック
	char name2[STR_MAX];
	strcpy_s(name2, STR_MAX, filename);
	name2[strlen(name2) - 3] = 'B';
	name2[strlen(name2) - 2] = 'I';
	name2[strlen(name2) - 1] = 'N';

	
	strcpy_s(mBinFile, STR_MAX, name2);
	if (LoadBIN(name2))return;

	FbxManager* manager = FbxManager::Create();

	// ファイルからシーンに読み込み
	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename);
	FbxScene* scene = FbxScene::Create(manager, "");
	importer->Import(scene);
	
	//// モーション情報取得
	//FbxArray<FbxString*> names;
	//scene->FillAnimStackNameArray(names);
	int animStackCount = importer->GetAnimStackCount();
	if (animStackCount > 0)
	{
		// モーションが存在するとき
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

	//if (scene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::DirectX)
	//{
	//	FbxAxisSystem::DirectX.ConvertScene(scene);
	//}

	// モデルを3角形に分割
	fbxsdk::FbxGeometryConverter geometryConverter(manager);
	//geometryConverter.SplitMeshesPerMaterial(scene, true);
	geometryConverter.Triangulate(scene, true);

	// メッシュ数取得
	mMeshNum = scene->GetSrcObjectCount<fbxsdk::FbxMesh>();

	// 頂点数計算
	int vertexNum = 0; // 合計頂点数
	for (int i = 0; i < mMeshNum; ++i)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(i);
		int num = mesh->GetPolygonVertexCount();
		vertexNum += num;
	}

	// 頂点確保
	mVertices = new Vertex[vertexNum];
	mIndices = new u_int[vertexNum];
	mWeights = new Weight[vertexNum];

	// マテリアルごとのポリゴン超点数
	mMaterials = new Material[mMeshNum];
	mMaterialColors = new Vector4[mMeshNum];

	// 頂点読み込み
	mVerticesNum = 0;
	mBoneNum = 0;
	for (int m = 0; m < mMeshNum; ++m)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(m);
		int num = mesh->GetPolygonVertexCount();
		
		// 座標
		int* index = mesh->GetPolygonVertices();
		FbxVector4* source = mesh->GetControlPoints();

		FbxAMatrix globalTrans = mesh->GetNode()->EvaluateGlobalTransform();
		// 全頂点変換
		for (int v = 0; v < mesh->GetControlPointsCount(); ++v)
		{
			source[v] = globalTrans.MultT(source[v]);
		}

		// 頂点座標読み込み
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

		// 法線読み込み
		FbxArray<FbxVector4> normal;
		mesh->GetPolygonVertexNormals(normal);
		for (int v = 0; v < normal.Size(); ++v)
		{
			mVertices[v + mVerticesNum].normal.x = (float)normal[v][0];
			mVertices[v + mVerticesNum].normal.y = (float)normal[v][1];
			mVertices[v + mVerticesNum].normal.z = (float)normal[v][2];
		}

		// UV読み込み
		FbxStringList names;
		mesh->GetUVSetNames(names);
		FbxArray<FbxVector2> uv;
		mesh->GetPolygonVertexUVs(names.GetStringAt(0), uv);
		for (int v = 0; v < uv.Size(); ++v)
		{
			mVertices[v + mVerticesNum].tex.x = (float)uv[v][0];
			mVertices[v + mVerticesNum].tex.y = (float)(1.0f - uv[v][1]);
		}

		// 接線
		FbxGeometryElementTangent* element = mesh->CreateElementTangent();
		FbxLayerElement::EMappingMode mapmode = element->GetMappingMode();
		FbxLayerElement::EReferenceMode refmode = element->GetReferenceMode();

		//    ポリゴン頂点に対するインデックス参照形式のみ対応
		if (mapmode == FbxGeometryElement::eByPolygonVertex)
		{
			if (refmode == FbxGeometryElement::eIndexToDirect)
			{
				FbxLayerElementArrayTemplate<FbxVector4>* index = &element->GetDirectArray();
				int indexCount = index->GetCount();
				for (int j = 0; j < indexCount; j++) {
					// FbxColor取得
					FbxVector4 v = index->GetAt(j);
					// DWORD型のカラー作成        
					mVertices[j + mVerticesNum].tangent.x = (float)v[0];
					mVertices[j + mVerticesNum].tangent.y = (float)v[1];
					mVertices[j + mVerticesNum].tangent.z = (float)v[2];
				}

				//FbxLayerElementArrayTemplate<int>* index = &element->GetIndexArray();
				//int indexCount = index->GetCount();
				//for (int j = 0; j < indexCount; j++) {
				// 
				//	// FbxColor取得
				//	FbxVector4 v = element->GetDirectArray().GetAt(index->GetAt(j));
				//	// DWORD型のカラー作成        
				//	mVertices[j + mVerticesNum].tangent.x = (float)v[0];
				//	mVertices[j + mVerticesNum].tangent.y = (float)v[1];
				//	mVertices[j + mVerticesNum].tangent.z = (float)v[2];
				//}
			}
		}
		// 
		// 従法線

		// インデックス設定
		for (int k = 0; k < num; k += 3)
		{
			mIndices[k + 0 + mVerticesNum] = k + 0 + mVerticesNum;
			mIndices[k + 1 + mVerticesNum] = k + 1 + mVerticesNum;
			mIndices[k + 2 + mVerticesNum] = k + 2 + mVerticesNum;
		}

		// ボーン読み込み
		LoadBone(mesh);

		
		// メッシュの使用マテリアル取得
		FbxLayerElementMaterial* lem = mesh->GetElementMaterial();
		if (lem != NULL)
		{
			// ポリゴンに貼られているマテリアル番号
			int materialIndex = lem->GetIndexArray().GetAt(0);

			// メッシュマテリアルのmaterialIndex番目を取得
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

			// テクスチャ読み込み
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
				// 相対パスに変換
				size_t len = strnlen_s(path, STR_MAX);
				const char* imgName = &path[len];
				for (size_t i = 0; i < len; ++i)
				{
					imgName--;
					if (*imgName == '/') { ++imgName; break; }
					if (*imgName == '\\') { ++imgName; break; }
				}
				char relativeName[STR_MAX] = {};
				strcpy_s(relativeName, STR_MAX, mFbxDir); // fbxのパスをコピー
				// strcat_s(relativeName, STR_MAX, "texture/"); // textureフォルダ
				strcat_s(relativeName, STR_MAX, imgName); // pngのファイル名を連結

				// ワイド文字に変換
				wchar_t filename[STR_MAX];
				setlocale(LC_ALL, "Japanese_Japan.932");
				size_t ret = 0;
				mbstowcs_s(&ret, mMaterials[m].filename, STR_MAX, imgName, _TRUNCATE);
				mbstowcs_s(&ret, filename, STR_MAX, relativeName, _TRUNCATE);

				// テクスチャ読み込み
				mMaterials[m].diffuse.Load(filename);

				// ノーマルマップ
				char normalMapName[STR_MAX] = {};
				strcpy_s(normalMapName, STR_MAX, mFbxDir); // fbxのパスをコピー
				strcat_s(normalMapName, STR_MAX, "N"); // pngのファイル名を連結
				strcat_s(normalMapName, STR_MAX, imgName); // pngのファイル名を連結
				mbstowcs_s(&ret, filename, STR_MAX, normalMapName, _TRUNCATE);

				mMaterials[m].normal.Load(filename);
			}
			else
			{
				mMaterials[m].diffuse.Load(L"");
				mMaterials[m].normal.Load(L"");
			}




		}
		else
		{
			mMaterials[m].diffuse.Load(L"");
			mMaterials[m].normal.Load(L"");
		}

		mMaterials[m].faceNum = num / 3;

		mVerticesNum += num;
	}

	mFaceNum = mVerticesNum / 3;
	OptimizeVertices();

	// 頂点元データ保存
	mVerticesSource = new VertexForSkinning[mVerticesNum];
	for (int i = 0; i < mVerticesNum; ++i)
	{
		mVerticesSource[i].pos = mVertices[i].pos;
		mVerticesSource[i].normal = mVertices[i].normal;
	}

	// ウェイト正規化
	// 5本以上にまたがってる場合のため
	for (int i = 0; i < mVerticesNum; ++i)
	{
		float n = 0;

		// 頂点のウェイトの合計値
		for (int w = 0; w < mWeights[i].count; ++w)
		{
			n += mWeights[i].weight[w];
		}

		// 正規化
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
	// メッシュ頂点数
	int num = mesh->GetPolygonVertexCount();

	// スキン情報の有無
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0)
	{
		return;
	}

	FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));

	// ボーン数
	int boneNum = skin->GetClusterCount();

	// 全ボーン情報取得
	for (int bone = 0; bone < boneNum; ++bone)
	{
		// ボーン情報取得
		FbxCluster* cluster = skin->GetCluster(bone);
		FbxAMatrix trans;
		cluster->GetTransformMatrix(trans);

		// ボーン名
		const char* boneName = cluster->GetLink()->GetName();

		// ボーン検索
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

			// オフセット行列作成
			FbxAMatrix linkMatrix;
			cluster->GetTransformLinkMatrix(linkMatrix);

			FbxAMatrix offset = linkMatrix.Inverse() * trans;
			FbxDouble* offsetM = (FbxDouble*)offset;

			// オフセット行列保存
			for (int i = 0; i < 16; ++i)
			{
				mBones[boneNo].offsetMatrix.m[i] = (float)offsetM[i];
			}

			// キーフレーム読み込み
			LoadKeyFrames(DEFAULT, boneNo, cluster->GetLink());
		}

		int weightCount = cluster->GetControlPointIndicesCount();
		int* weightIndex = cluster->GetControlPointIndices();
		double* weight = cluster->GetControlPointWeights();

		int* index = mesh->GetPolygonVertices();

		for (int i = 0; i < weightCount; ++i)
		{
			int weightIndex2 = weightIndex[i];

			// 全ポリゴンからweightIndex2番目の頂点検索
			for (int v = 0; v < num; ++v)
			{
				if (index[v] != weightIndex2) continue;

				// 頂点にウェイトを保存
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
	int bone = -1; // 見つからない
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

	// 新バッファ確保
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

		// t秒の姿勢行列をGet
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

	// 配列用変数
	int f = (int)mFrame;
	int nf = f + 1; // nextFrame
	if (f == motion->frameNum - 1) // fが最後のフレームなら
	{
		nf = 0;
	}

	Matrix keyMatrix[BONE_MAX];

	auto MatrixInterporate = [](Matrix& out, const Matrix& A, const Matrix& B, float rate)
	{
		//	ボーン行列の補間
		out = A * (1.0f - rate) + B * rate;
	};

	// 行列準備
	for (int b = 0; b < mBoneNum; ++b)
	{
		// 行列補完
		Matrix mat;
		MatrixInterporate(mat, motion->keyframe[b][f], motion->keyframe[b][nf], mFrame - f);
		mBones[b].transform = mat;

		//キーフレーム
		keyMatrix[b] = mBones[b].offsetMatrix * mat;
	}

	mSkinningShader->UpdateStructureBuffer(keyMatrix, 2);

	CbufferForSkinning cb;
	{
		//定数バッファ更新
		const float numThreadX = 256.0f;
		cb.groupNumX = static_cast<UINT>(ceil(mVerticesNum / numThreadX));
		cb.groupNumY = static_cast<UINT>(1);
		cb.groupNumZ = static_cast<UINT>(1);
		cb.verticesNum = mVerticesNum;
		mSkinningShader->UpdateConstantBuffer(&cb);
	}
	
	mSkinningShader->Run(cb.groupNumX, cb.groupNumY, cb.groupNumZ);

	//// 計算結果を読み込む
	//{

	//	ID3D11Buffer* result = nullptr;
	//	mSkinningShader->CreateResultBuffer(&result);

	//	HRESULT hr = S_OK;

	//	D3D11_MAPPED_SUBRESOURCE ms;
	//	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//	hr = context->Map(result, 0, D3D11_MAP_READ, 0, &ms);
	//	if (FAILED(hr)) return;

	//	auto start = std::chrono::system_clock::now();
	//	VertexForSkinning* v = static_cast<VertexForSkinning*>(ms.pData);
	//	for (int i = 0; i < mVerticesNum; ++i)
	//	{
	//		mVertices[i].pos    = v[i].pos;
	//		mVertices[i].normal = v[i].normal;
	//	}

	//	context->Unmap(result, 0);

	//	result->Release();
	//	auto end = std::chrono::system_clock::now();

	//	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	//	std::wstring wstr = std::to_wstring(elapsed);
	//	FONT.Set(wstr.c_str(), Vector2::Zero());
	//}

	//FRAMEWORK.GetContext()->UpdateSubresource(mVertexBuffer.Get(), 0, NULL, mVertices, 0, 0);
	Animate(1.0f / 60.0f);

	//// 頂点変形
	//for (int v = 0; v < mVerticesNum; ++v)
	//{
	//	// 頂点 * ボーン行列
	//	// b = v番目の頂点の影響ボーン
	//	if (mWeights[v].count <= 0)continue;
	//	mVertices[v].pos.x = 0;
	//	mVertices[v].pos.y = 0;
	//	mVertices[v].pos.z = 0;
	//	// 影響個数分ループ
	//	for (int n = 0; n < mWeights[v].count; ++n)
	//	{
	//		int b = mWeights[v].bone[n];
	//		float x = mVerticesSource[v].pos.x;
	//		float y = mVerticesSource[v].pos.y;
	//		float z = mVerticesSource[v].pos.z;
	//		// 座標を影響力分移動
	//		mVertices[v].pos.x += (x * keyMatrix[b]._11 + y * keyMatrix[b]._21 + z * keyMatrix[b]._31 + 1 * keyMatrix[b]._41) * mWeights[v].weight[n];
	//		mVertices[v].pos.y += (x * keyMatrix[b]._12 + y * keyMatrix[b]._22 + z * keyMatrix[b]._32 + 1 * keyMatrix[b]._42) * mWeights[v].weight[n];
	//		mVertices[v].pos.z += (x * keyMatrix[b]._13 + y * keyMatrix[b]._23 + z * keyMatrix[b]._33 + 1 * keyMatrix[b]._43) * mWeights[v].weight[n];
	//		float nx = mVerticesSource[v].normal.x;
	//		float ny = mVerticesSource[v].normal.y;
	//		float nz = mVerticesSource[v].normal.z;
	//		//	法線を影響力分変換
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

	// オフセット行列
	mBones[boneNo].offsetMatrix.Identity();

	// キーフレーム読み込み
	LoadKeyFrames(DEFAULT, boneNo, node);

	// ウェイト設定
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

	// 頂点数
	fread(&mVerticesNum, sizeof(int), 1, fp);

	// 頂点読み込み
	mVertices = new Vertex[mVerticesNum];
	fread(mVertices, sizeof(Vertex), mVerticesNum, fp);

	// AABB
	fread(&mAABB.min, sizeof(Vector3), 1, fp);
	fread(&mAABB.max, sizeof(Vector3), 1, fp);
	
	// 面数
	fread(&mFaceNum, sizeof(int), 1, fp);

	// インデックス
	mIndices = new u_int[mFaceNum * 3];
	fread(mIndices, sizeof(u_int), mFaceNum * 3, fp);

	// メッシュ数
	fread(&mMeshNum, sizeof(int), 1, fp);

	// マテリアル数 <ファイル名、面数>
	mMaterials = new Material[mMeshNum];
	for (int i = 0; i < mMeshNum ;++i)
	{
		fread(mMaterials[i].filename, sizeof(wchar_t) * STR_MAX, 1, fp);
		fread(&mMaterials[i].faceNum, sizeof(int), 1, fp);

		// ワイド文字に変換
		wchar_t filepath[STR_MAX];
		wchar_t nFilepath[STR_MAX];
		setlocale(LC_ALL, "Japanese_Japan.932");
		size_t ret = 0;
		mbstowcs_s(&ret, filepath, STR_MAX, mFbxDir, _TRUNCATE);
		wcscpy_s(nFilepath, STR_MAX, filepath);
		wcscat_s(nFilepath, STR_MAX, L"N");
		wcscat_s(filepath, STR_MAX, mMaterials[i].filename);
		wcscat_s(nFilepath, STR_MAX, mMaterials[i].filename);
		mMaterials[i].diffuse.Load(filepath);
		mMaterials[i].normal.Load(nFilepath);
	}

	// マテリアルカラー
	mMaterialColors = new Vector4[mMeshNum];
	fread(mMaterialColors, sizeof(DirectX::XMFLOAT4), mMeshNum, fp);

	// ボーン
	fread(&mBoneNum, sizeof(int), 1, fp);
	fread(mBones, sizeof(Bone), mBoneNum, fp);

	// ウェイト
	mWeights = new Weight[mVerticesNum];
	fread(mWeights, sizeof(Weight), mVerticesNum, fp);

	// 頂点元データ
	mVerticesSource = new VertexForSkinning[mVerticesNum];
	for (int i = 0; i < mVerticesNum; ++i)
	{
		mVerticesSource[i].pos = mVertices[i].pos;
		mVerticesSource[i].normal = mVertices[i].normal;
	}

	// モーション
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

	// 頂点
	fwrite(&mVerticesNum, sizeof(int), 1, fp);
	fwrite(mVertices, sizeof(Vertex), mVerticesNum, fp);

	// AABB
	fwrite(&mAABB.min, sizeof(Vector3), 1, fp);
	fwrite(&mAABB.max, sizeof(Vector3), 1, fp);

	// インデックス
	fwrite(&mFaceNum, sizeof(int), 1, fp);
	fwrite(mIndices, sizeof(u_int), mFaceNum * 3, fp);

	// メッシュ数 
	fwrite(&mMeshNum, sizeof(int), 1, fp);

	// マテリアル数 <ファイル名、面数>、
	for (int i = 0; i < mMeshNum; ++i)
	{
		fwrite(mMaterials[i].filename, sizeof(wchar_t) * STR_MAX, 1, fp);
		fwrite(&mMaterials[i].faceNum, sizeof(int), 1, fp);
	}
	// マテリアルカラー
	fwrite(mMaterialColors, sizeof(Vector4), mMeshNum, fp);

	// ボーン
	fwrite(&mBoneNum, sizeof(int), 1, fp);
	fwrite(mBones, sizeof(Bone), mBoneNum, fp);

	// ウェイト保存
	fwrite(mWeights, sizeof(Weight), mVerticesNum, fp);

	// モーション保存
	size_t motionNum = mMotion.size();
	fwrite(&motionNum, sizeof(size_t), 1, fp);
	for (auto& motion : mMotion)
	{
		// key1(MotionType)保存 
		fwrite(&motion.first, sizeof(int), 1, fp);

		// key2(Motion)保存
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
		mMaterials[i].diffuse.UnLoad();
		mMaterials[i].normal.UnLoad();
	}
	delete[] mVertices;
	delete[] mVerticesSource;
	delete[] mIndices;
	delete[] mMaterials;
	delete[] mMaterialColors;
	delete[] mWeights;


	// モーション関連解放
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
		//mSkinningShader->CreateRWStructuredBuffer(mVerticesNum, sizeof(VertexForSkinning));
		mSkinningShader->CreateRWByteaddressBuffer(mVertices, mVerticesNum, sizeof(Vertex));
		mSkinningShader->CreateConstantBuffer(sizeof(CbufferForSkinning));

		mVertexBuffer = mSkinningShader->GetRWBuffer();
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
		mMaterials[m].diffuse.Set(0);
		mMaterials[m].normal.Set(1);

		Cbuffer cb;
		DirectX::XMStoreFloat4x4(&cb.wvp,  DirectX::XMLoadFloat4x4(&wvp));
		DirectX::XMStoreFloat4x4(&cb.world, DirectX::XMLoadFloat4x4(&world));
		cb.lightDirection = lightDirection;
		cb.materialColor = mMaterialColors[m];
		context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, &cb, 0, 0);
		context->VSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());

		// 描画
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
		mMaterials[m].diffuse.Set(0);
		mMaterials[m].normal.Set(1);

		Cbuffer cb;
		DirectX::XMStoreFloat4x4(&cb.wvp, DirectX::XMLoadFloat4x4(&wvp));
		DirectX::XMStoreFloat4x4(&cb.world, DirectX::XMLoadFloat4x4(&world));
		cb.lightDirection = lightDirection;
		cb.materialColor = mMaterialColors[m];
		context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, &cb, 0, 0);
		context->VSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());

		// 描画
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

	// 高速化のため
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

		// 面頂点取得
		int index0 = mIndices[i * 3 + 0];
		vertex[0] = DirectX::XMLoadFloat3(&mVertices[index0].pos); // a

		int index1 = mIndices[i * 3 + 1];
		vertex[1] = DirectX::XMLoadFloat3(&mVertices[index1].pos); // b 

		int index2 = mIndices[i * 3 + 2];
		vertex[2] = DirectX::XMLoadFloat3(&mVertices[index2].pos); // c

		// 3辺算出
		DirectX::XMVECTOR ab = DirectX::XMVectorSubtract(vertex[1], vertex[0]);
		DirectX::XMVECTOR bc = DirectX::XMVectorSubtract(vertex[2], vertex[1]);
		DirectX::XMVECTOR ca = DirectX::XMVectorSubtract(vertex[0], vertex[2]);

		// 外積による法線算出
		nor = DirectX::XMVector3Cross(ab, bc);
		nor = DirectX::XMVector3Normalize(nor);

		// 内積の結果がプラスならば裏向き
		dot = DirectX::XMVector3Dot(nor, dir);
		if (DirectX::XMVectorGetX(dot) > 0) continue;

		// 交点算出
		v0 = DirectX::XMVectorSubtract(vertex[0], start); // ポリゴンの1頂点からレイの開始地点のベクトル
		d1 = DirectX::XMVector3Dot(v0, nor);				// ↑のベクトルとポリゴンの法線の内積(射影を出す)
		t = DirectX::XMVectorDivide(d1, dot);				//  ↑のベクトルと法線の割合をだす
		float ft = 0.0f;
		DirectX::XMStoreFloat(&ft, t);
		if (ft < 0.0f || ft > neart) continue;

		// 交点
		cp = DirectX::XMVectorAdd(start, DirectX::XMVectorMultiply(dir, t));

		// 内点判定
		// 1つ目
		tempV = DirectX::XMVectorSubtract(vertex[0], cp); //pa
		tempN = DirectX::XMVector3Cross(tempV, ab);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // 内積の結果マイナスなら外側

		// 2つ目
		tempV = DirectX::XMVectorSubtract(vertex[1], cp); //pb
		tempN = DirectX::XMVector3Cross(tempV, bc);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // 内積の結果マイナスなら外側

		// 3つ目
		tempV = DirectX::XMVectorSubtract(vertex[2], cp); //pc
		tempN = DirectX::XMVector3Cross(tempV, ca);
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(tempN, nor)) < 0) continue; // 内積の結果マイナスなら外側

		// 情報保存
		position = cp;
		normal = nor;
		neart = ft;
		ret = i; // i番目のポリゴンにヒット
	}
	if (ret != -1)
	{
		// 当たった
		DirectX::XMStoreFloat3(outPos, position);
		DirectX::XMStoreFloat3(outNormal, normal);
	}

	// 最も近いヒット位置までの距離
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
	// すでにキーが登録されてたらreturn
	if (mMotion.count(type) >= 1) return;

	FbxManager* manager = FbxManager::Create();
	FbxScene* scene = FbxScene::Create(manager, "");

	// ファイルからシーンに読み込み
	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(filename);
	importer->Import(scene);
	importer->Destroy();

	// モーション情報取得
	FbxArray<FbxString*> names;
	scene->FillAnimStackNameArray(names);

	FbxTakeInfo* take = scene->GetTakeInfo(names[0]->Buffer());
	FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
	FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
	FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);

	mStartFrame = (int)(start / fps60);
	mMotion[type].frameNum = (int)((stop - start) / fps60);
	
	FbxNode* root = scene->GetRootNode();

	// 全ボーン読み込み
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

	// 更新
	Save(mBinFile);
}

void SkinnedMesh::Animate(float elapsedTime)
{
	// モーション時間の更新
	mFrame += elapsedTime * 60;

	// ループチェック
	if (mFrame >= mMotion[mMotionType].frameNum - 1)
	{
		mFrame = 0;

		if (!mIsLoop) SetMotion(IDLE, true); 
	}
}


