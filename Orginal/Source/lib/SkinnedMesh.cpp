#include "SkinnedMesh.h"

#include "Framework.h"

#include "ConvertString.h"
#include "Font.h"
#include "Math.h"
#include "ResourceManager.h"

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
	//int animStackCount = importer->GetAnimStackCount();
	//if (animStackCount > 0)
	//{
	//	// モーションが存在するとき
	//	//FbxTakeInfo* take = importer->GetTakeInfo(0);
	//	//FbxLongLong start = take->mLocalTimeSpan.GetStart().Get();
	//	//FbxLongLong stop = take->mLocalTimeSpan.GetStop().Get();
	//	//FbxLongLong fps60 = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
	//	//mStartFrame = (int)(start / fps60);
	//	//mMotion[T_POSE_INDEX].frameNum = (int)((stop - start) / fps60);
	//	//SetMotion(T_POSE_INDEX, 0.1f);
	//}
	//else
	//{
	//	mStartFrame = 0;
	//	mMotion[T_POSE_INDEX].frameNum = 0;
	//}
	mMotion[T_POSE_INDEX].frameNum = 1;

	importer->Destroy();


	//FbxAxisSystem axis = scene->GetGlobalSettings().GetAxisSystem();

	// モデルを3角形に分割
	fbxsdk::FbxGeometryConverter geometryConverter(manager);
	geometryConverter.SplitMeshesPerMaterial(scene, true);
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

		//FbxVector4 T = mesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
		//FbxVector4 R = mesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
		//FbxVector4 S = mesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);
		//FbxAMatrix TRS = FbxAMatrix(T, R, S);
		//	全頂点変換
		//for (int v = 0; v < mesh->GetControlPointsCount(); v++) {
		//	source[v] = TRS.MultT(source[v]);
		//}

		//FbxAMatrix globalTrans = mesh->GetNode()->EvaluateGlobalTransform();
		//1 全頂点変換
		//for (int v = 0; v < mesh->GetControlPointsCount(); ++v)
		//{
		//	source[v] = globalTrans.MultT(source[v]);
		//}

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
		{
			if (mesh->GetElementTangentCount() > 0)
			{
				FbxGeometryElementTangent* element = mesh->GetElementTangent();// mesh->CreateElementTangent();
				//FbxLayerElement::EMappingMode mapmode = element->GetMappingMode();
				//FbxLayerElement::EReferenceMode refmode = element->GetReferenceMode();

				FbxLayerElementArrayTemplate<FbxVector4>* index = &element->GetDirectArray();
				int indexCount = index->GetCount();
				for (int i = 0; i < indexCount; i++) {
					// FbxColor取得
					FbxVector4 v = index->GetAt(i);
					// DWORD型のカラー作成        
					mVertices[i + mVerticesNum].tangent.x = (float)v[0];
					mVertices[i + mVerticesNum].tangent.y = (float)v[1];
					mVertices[i + mVerticesNum].tangent.z = (float)v[2];
				}
			}
			else
			{
				// tangent上方がなかったら仮のtangentを入れとく
				for (int i = 0; i < num; ++i)
				{
					mVertices[i + mVerticesNum].tangent.x = 0.0f;
					mVertices[i + mVerticesNum].tangent.y = 1.0f;
					mVertices[i + mVerticesNum].tangent.z = 0.0f;
				}
			}


		}

		// 従法線
		{
			if (mesh->GetElementBinormalCount() > 0)
			{
				FbxGeometryElementBinormal* element = mesh->GetElementBinormal();// mesh->CreateElementTangent();
				//FbxLayerElement::EMappingMode mapmode = element->GetMappingMode();
				//FbxLayerElement::EReferenceMode refmode = element->GetReferenceMode();

				FbxLayerElementArrayTemplate<FbxVector4>* index = &element->GetDirectArray();
				int indexCount = index->GetCount();
				for (int j = 0; j < indexCount; j++) {
					// FbxColor取得
					FbxVector4 v = index->GetAt(j);
					// DWORD型のカラー作成        
					mVertices[j + mVerticesNum].binormal.x = (float)v[0];
					mVertices[j + mVerticesNum].binormal.y = (float)v[1];
					mVertices[j + mVerticesNum].binormal.z = (float)v[2];
				}
			}
			else
			{
				// binormal情報がなかったら仮のbinormalを入れとく
				for (int i = 0; i < num; ++i)
				{
					auto& vertex = mVertices[i + mVerticesNum];
					vertex.binormal = Vector3::Cross(vertex.normal, vertex.tangent);
				}
			}

		}

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
			auto LoadTexture = [](FbxProperty* prop, MaterialSprite* spr, char fbxDir[STR_MAX])
			{
				std::string path; // 絶対パス取得用

				int fileTextureCount = prop->GetSrcObjectCount<FbxFileTexture>();
				if (fileTextureCount > 0)
				{
					FbxFileTexture* fileTex = prop->GetSrcObject<FbxFileTexture>(0);
					path = fileTex->GetFileName(); // 絶対パス
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

				// 空じゃないなら
				if (!path.empty())
				{
					// 絶対パスからテクスチャファイル名を抜く
					size_t len = path.size() - 1;
					const char* imgName = &path[len];
					for (size_t i = 0; i < len; ++i)
					{
						imgName--;
						if (*imgName == '/') { ++imgName; break; }
						if (*imgName == '\\') { ++imgName; break; }
					}

					// 相対パス作成
					std::string work = fbxDir; // AA/BB/
					work += imgName;		   // AA/BB/*.png
					std::wstring relative = ConvertString::ConvertToWstring(work);

					// 読み込み
					spr->sprite.Load(relative.c_str());
					wcscpy_s(spr->filename, STR_MAX, relative.c_str());
				}
				else
				{
					// 空だったらダミーテクスチャを読むようにする
					const wchar_t* dummy = L"";
					spr->sprite.Load(dummy);
					wcscpy_s(spr->filename, STR_MAX, dummy);
				}
			};

			if (prop.IsValid()) LoadTexture(&prop, &mMaterials[m].mtlSpr[0], mFbxDir); // Diffuse

			prop = material->FindProperty(FbxSurfaceMaterial::sNormalMap);
			if (prop.IsValid()) LoadTexture(&prop, &mMaterials[m].mtlSpr[1], mFbxDir); // NormalMap
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

	//Save(mBinFile);
}

void SkinnedMesh::InitializeBone(FbxScene* scene)
{
	// ボーン数取得
	mBoneNum = scene->GetSrcObjectCount<FbxSkeleton>();

	// ボーン初期設定
	for (int bone = 0; bone < mBoneNum; ++bone)
	{
		FbxSkeleton* skel = scene->GetSrcObject<FbxSkeleton>(bone);
		FbxNode* node = skel->GetNode();

		// ボーン名取得
		const char* name = node->GetName();
		strcpy_s(mBones[bone].name, STR_MAX, name);

		// 親設定
		mBones[bone].parent = nullptr;

		FbxNode* parent = node->GetParent();
		if (parent)
		{
			const char* parentName = parent->GetName();
			if (parentName)
			{
				int parentNo = FindBone(parentName);
				if (parentNo != -1)
				{
					mBones[bone].parent = &mBones[parentNo];
				}
			}
		}
	}

	// ボーンのオフセット行列作成
	int meshNum = scene->GetSrcObjectCount<FbxMesh>();
	auto& motion = mMotion[T_POSE_INDEX];
	for (int m = 0; m < meshNum; ++m)
	{
		FbxMesh* mesh = scene->GetSrcObject<FbxMesh>(m);
		FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
		if (!skin) continue;

		// skinのボーン数取得
		int nBone = skin->GetClusterCount();

		for (int bone = 0; bone < nBone; ++bone)
		{
			//	ボーン情報取得
			FbxCluster* cluster = skin->GetCluster(bone);
			FbxAMatrix trans;
			cluster->GetTransformMatrix(trans);

			//	ボーン検索
			const char* name = cluster->GetLink()->GetName();
			int bone_no = FindBone(name);
			if (bone_no < 0) continue;

			//	オフセット行列作成
			FbxAMatrix LinkMatrix;
			cluster->GetTransformLinkMatrix(LinkMatrix);

			FbxAMatrix Offset = LinkMatrix.Inverse() * trans;
			FbxDouble* OffsetM = (FbxDouble*)Offset;
			//	オフセット行列保存
			for (int i = 0; i < 16; i++) {
				mBones[bone_no].offsetMatrix.m[i] = (float)OffsetM[i];
			}

			// TPOSE
			if (!motion.keyframe[bone_no])
			{
				//motion.keyframe[bone_no] = new Matrix[motion.frameNum];
				//FbxNode* node = cluster->GetLink();

				LoadKeyFrames(T_POSE_INDEX, bone_no, cluster->GetLink());

				//double time = mStartFrame * (1.0 / 60);
				//FbxTime t;
				//t.SetSecondDouble(time);

				//// t秒の姿勢行列をGet
				//FbxMatrix m = node->EvaluateGlobalTransform(t);

				//FbxDouble* mat = (FbxDouble*)m;
				//for (int i = 0; i < 16; ++i)
				//{
				//	mMotion[T_POSE_INDEX].keyframe[bone_no][0].m[i] = (float)mat[i];
				//}
			}
		}


	}
}

void SkinnedMesh::LoadBone(FbxMesh* mesh)
{
	// メッシュ頂点数
	int num = mesh->GetPolygonVertexCount();

	// スキン情報の有無
	int skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount <= 0) return;

	FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));

	// ボーン数
	int boneNum = skin->GetClusterCount();

	// 全ボーン情報取得
	for (int bone = 0; bone < boneNum; ++bone)
	{
		// ボーン情報取得
		FbxCluster* cluster = skin->GetCluster(bone);

		// ボーン名
		FbxNode* node = cluster->GetLink();
		const char* boneName = node->GetName();

		// ボーン検索
		bool isNewBorn = false;
		int boneNo = FindBone(boneName);
		if (boneNo < 0)
		{
			boneNo = mBoneNum;
			++mBoneNum;
			isNewBorn = true;
		}

		if (isNewBorn)
		{
			// ボーン名を設定
			strcpy_s(mBones[boneNo].name, STR_MAX, boneName);

			// 親を設定
			mBones[boneNo].parent = nullptr;

			FbxNode* parent = node->GetParent();
			if (parent)
			{
				const char* parentName = parent->GetName();
				if (parentName)
				{
					int parentNo = FindBone(parentName);
					if (parentNo != -1)
					{
						mBones[boneNo].parent = &mBones[parentNo];
					}
				}
			}

			//	オフセット行列作成
			FbxAMatrix trans;
			cluster->GetTransformMatrix(trans);
			
			FbxAMatrix LinkMatrix;
			cluster->GetTransformLinkMatrix(LinkMatrix);
			
			FbxAMatrix Offset = LinkMatrix.Inverse() * trans;
			FbxDouble* OffsetM = (FbxDouble*)Offset;
			for (int i = 0; i < 16; i++) {
				mBones[boneNo].offsetMatrix.m[i] = (float)OffsetM[i];
			}

			LoadKeyFrames(T_POSE_INDEX, boneNo, node);
		}
		if (boneNo < 0) continue;


		// ウェイト読み込み
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

		//LoadKeyFrames(T_POSE_INDEX, boneNo, cluster->GetLink());
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

void SkinnedMesh::LoadKeyFrames(int type, int bone, FbxNode* boneNode)
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

void SkinnedMesh::Skinning(float elapsedTime)
{
	// 現在のモーション取得
	Motion* cur = &mMotion[mMotionType];
	if (cur == NULL) return;

	// モーションブレンド
	Matrix result[BONE_MAX];
	if (mBlendFactor < 1.0f)
	{
		// 前のモーション取得
		Motion* old = &mMotion[mBeforeMotionType];
		Matrix blend = {};

		// ブレンドキーフレーム作成
		for (int i = 0; i < mBoneNum; ++i)
		{
			// キーフレーム取得
			Matrix* key1 = &old->keyframe[i][(int)mBeforeFrame];
			Matrix* key2 = &cur->keyframe[i][(int)mFrame];

			// ブレンド
			Matrix::Interporate(&blend, *key1, *key2, mBlendFactor);
			mBones[i].transform = blend;

			// ボーンの行列適用
			result[i] = mBones[i].offsetMatrix * blend;
		}

		// ブレンド率更新
		mBlendFactor = Math::Clamp01(mBlendFactor + mBlendSpeed);
	}
	else
	{
		// 現在のモーションを代入する
		for (int i = 0; i < mBoneNum; ++i)
		{
			int f = (int)mFrame;

			Matrix mat;
			Matrix::Interporate(&mat, cur->keyframe[i][f], cur->keyframe[i][(f + 1) % (cur->frameNum + 1)], mFrame - f);
			result[i] = mBones[i].offsetMatrix * mat;


			mBones[i].transform = mat;
		}
	}

	// キーフレーム情報更新
	mSkinningShader->UpdateStructureBuffer(result, 2);

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
	Animate(elapsedTime);
}

void SkinnedMesh::LoadMeshAnim(FbxMesh* mesh)
{
	FbxNode* node = mesh->GetNode();

	int boneNo = mBoneNum;
	strcpy_s(mBones[boneNo].name, STR_MAX, node->GetName());

	// オフセット行列
	mBones[boneNo].offsetMatrix.Identity();

	// キーフレーム読み込み
	LoadKeyFrames(T_POSE_INDEX, boneNo, node);

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
	for (int m = 0; m < mMeshNum; ++m)
	{
		for (int i = 0; i < Material::KIND; ++i)
		{
			fread(mMaterials[m].mtlSpr[i].filename, sizeof(wchar_t) * STR_MAX, 1, fp);
			mMaterials[m].mtlSpr[i].sprite.Load(mMaterials[m].mtlSpr[i].filename); // 画像読み込み
		}

		fread(&mMaterials[m].faceNum, sizeof(int), 1, fp);
	}

	// マテリアルカラー
	mMaterialColors = new Vector4[mMeshNum];
	fread(mMaterialColors, sizeof(DirectX::XMFLOAT4), mMeshNum, fp);

	// ボーン
	fread(&mBoneNum, sizeof(int), 1, fp);
	for (int i = 0; i < mBoneNum; ++i)
	{
		auto& bone = mBones[i];
		fread(bone.name, sizeof(bone.name), 1, fp);
		fread(&bone.offsetMatrix, sizeof(Matrix), 1, fp);

		int parentNo = -1;
		fread(&parentNo, sizeof(int), 1, fp);
		if (parentNo < 0) bone.parent = nullptr;
		else			  bone.parent = &mBones[parentNo];
	}

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
		int type;
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
	if (fp == NULL) return;

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
	for (int m = 0; m < mMeshNum; ++m)
	{
		for (int i = 0; i < Material::KIND; ++i)
		{
			fwrite(mMaterials[m].mtlSpr[i].filename, sizeof(wchar_t) * STR_MAX, 1, fp);
		}

		fwrite(&mMaterials[m].faceNum, sizeof(int), 1, fp);
	}

	// マテリアルカラー
	fwrite(mMaterialColors, sizeof(Vector4), mMeshNum, fp);

	// ボーン
	fwrite(&mBoneNum, sizeof(int), 1, fp);
	for (int i = 0; i < mBoneNum; ++i)
	{
		auto& bone = mBones[i];
		fwrite(bone.name, sizeof(bone.name), 1, fp);
		fwrite(&bone.offsetMatrix, sizeof(Matrix), 1, fp);
		
		// 親のボーン番号
		int parentNo = -1;
		if (bone.parent) parentNo = FindBone(bone.parent->name);
		fwrite(&parentNo, sizeof(int), 1, fp);
	}

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

	Initialize(fbxFilename);
}

SkinnedMesh::~SkinnedMesh()
{
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
		mSkinningShader->CreateStructuredBuffer(mVerticesSource, mVerticesNum, sizeof(VertexForSkinning)); // 頂点ソース
		mSkinningShader->CreateStructuredBuffer(mWeights, mVerticesNum, sizeof(Weight));				   // ウェイト
		mSkinningShader->CreateStructuredBuffer(nullptr, mBoneNum, sizeof(Matrix));						   // ボーン行列
		mSkinningShader->CreateRWByteaddressBuffer(mVertices, mVerticesNum, sizeof(Vertex));
		mSkinningShader->CreateConstantBuffer(sizeof(CbufferForSkinning));

		mVertexBuffer = mSkinningShader->GetRWBuffer();
	}

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

void SkinnedMesh::Render(const Shader* shader, const Matrix& wvp, const Matrix& world, const Vector4& lightDir)
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
		cb.wvp = wvp;
		cb.world = world;
		cb.lightDir = lightDir;
		cb.materialColor = mMaterialColors[m];
		context->UpdateSubresource(mConstBuffer.Get(), 0, NULL, &cb, 0, 0);
		context->VSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, mConstBuffer.GetAddressOf());

		// 描画
		context->DrawIndexed(mMaterials[m].faceNum * 3, start, 0);
		start += mMaterials[m].faceNum * 3;
	}

	// CSでVBを使ってるのでリセット
	stride = 0;
	ID3D11Buffer* dummyVB = nullptr;
	context->IASetVertexBuffers(0, 1, &dummyVB, &stride, &offset);
}

int SkinnedMesh::RayPick(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal)
{
	int ret = -1; // ヒットした面番号
	float minDist = FLT_MAX; // 一番近いヒット面までの距離

	// 高速化のため
	Vector3 vertex[3] = {};
	Vector3 line[3] = {};
	Vector3 normal;
	Vector3 v0;
	Vector3 cp;

	Vector3 work;
	Vector3 interior;
	for (int i = 0; i < mFaceNum; ++i)
	{
		// 面頂点取得
		int index0 = mIndices[i * 3 + 0];
		vertex[0].x = mVertices[index0].pos.x;
		vertex[0].y = mVertices[index0].pos.y;
		vertex[0].z = mVertices[index0].pos.z;

		int index1 = mIndices[i * 3 + 1];
		vertex[1].x = mVertices[index1].pos.x;
		vertex[1].y = mVertices[index1].pos.y;
		vertex[1].z = mVertices[index1].pos.z;

		int index2 = mIndices[i * 3 + 2];
		vertex[2].x = mVertices[index2].pos.x;
		vertex[2].y = mVertices[index2].pos.y;
		vertex[2].z = mVertices[index2].pos.z;

		// 3辺算出
		// line = vertex - vertex;
		line[0].x = vertex[1].x - vertex[0].x;
		line[0].y = vertex[1].y - vertex[0].y;
		line[0].z = vertex[1].z - vertex[0].z;

		line[1].x = vertex[2].x - vertex[1].x;
		line[1].y = vertex[2].y - vertex[1].y;
		line[1].z = vertex[2].z - vertex[1].z;

		line[2].x = vertex[0].x - vertex[2].x;
		line[2].y = vertex[0].y - vertex[2].y;
		line[2].z = vertex[0].z - vertex[2].z;

		// 外積による法線算出
		// normal = line[0].Cross(line[1]);
		normal.x = line[0].y * line[1].z - line[0].z * line[1].y;
		normal.y = line[0].z * line[1].x - line[0].x * line[1].z;
		normal.z = line[0].x * line[1].y - line[0].y * line[1].x;
		normal.Normalize(); // 正規化

		// 法線をレイ方向に射影(内積)
		// dot = -normal.Dot(velocity);
		float dot = -normal.x * velocity.x + -normal.y * velocity.y + -normal.z * velocity.z;
		if (dot <= 0) continue;


		// ポリゴンの1頂点からレイの開始地点のベクトル
		v0.x = vertex[0].x - pos.x;
		v0.y = vertex[0].y - pos.y;
		v0.z = vertex[0].z - pos.z;

		// ↑のベクトルとポリゴンの法線の内積(射影を出す)
		// float dot2 = -normal.Dot(v0);
		float dot2 = -normal.x * v0.x + -normal.y * v0.y + -normal.z * v0.z;
		
		// レイの長さ
		float len = dot2 / dot;
		if (len < 0 || len > minDist) continue;
		
		// 交点算出
		cp.x = pos.x + velocity.x * len;
		cp.y = pos.y + velocity.y * len;
		cp.z = pos.z + velocity.z * len;


		// 内点判定
		// 1つ目
		work.x = vertex[0].x - cp.x;
		work.y = vertex[0].y - cp.y;
		work.z = vertex[0].z - cp.z;
		interior.x = work.y * line[0].z - work.z * line[0].y;
		interior.y = work.z * line[0].x - work.x * line[0].z;
		interior.z = work.x * line[0].y - work.y * line[0].x;
		
		float d = normal.x * interior.x + normal.y * interior.y + normal.z * interior.z;
		if (d < 0) continue; // 内積の結果マイナスなら外側

		// 2つ目
		work.x = vertex[1].x - cp.x;
		work.y = vertex[1].y - cp.y;
		work.z = vertex[1].z - cp.z;
		interior.x = work.y * line[1].z - work.z * line[1].y;
		interior.y = work.z * line[1].x - work.x * line[1].z;
		interior.z = work.x * line[1].y - work.y * line[1].x;

		d = normal.x * interior.x + normal.y * interior.y + normal.z * interior.z;
		if (d < 0) continue; // 内積の結果マイナスなら外側

		// 3つ目
		work.x = vertex[2].x - cp.x;
		work.y = vertex[2].y - cp.y;
		work.z = vertex[2].z - cp.z;
		interior.x = work.y * line[2].z - work.z * line[2].y;
		interior.y = work.z * line[2].x - work.x * line[2].z;
		interior.z = work.x * line[2].y - work.y * line[2].x;		
		
		d = normal.x * interior.x + normal.y * interior.y + normal.z * interior.z;
		if (d < 0) continue; // 内積の結果マイナスなら外側

		// nan チェック
		//if (isnan(DirectX::XMVectorGetX(cp))) continue;
		//if (isnan(DirectX::XMVectorGetY(cp))) continue;
		//if (isnan(DirectX::XMVectorGetZ(cp))) continue;

		// 情報保存
		*outPos = cp;		 // 交点
		*outNormal = normal; // 法線
		minDist = len;		 // 最短距離更新
		ret = i;			 // i番目のポリゴンにヒット
	}

	return ret;
}

Matrix* SkinnedMesh::GetBoneMatrix(const char* name)
{
	// ボーンNo取得
	int b = FindBone(name);
	if (b == -1) return nullptr;

	return &mBones[b].transform;
}

Matrix* SkinnedMesh::GetBoneMatrix(int boneIndex)
{
	return &mBones[boneIndex].transform;;
}

void SkinnedMesh::SetMotion(int type, float blendSpeed)
{
	// 同じモーションならreturn
	if (mMotionType == type) return;

	// 今のモーションがTPOSEじゃないなら(TPOSEからのブレンドはしない
	if (mMotionType != T_POSE_INDEX)
	{
		// 今のモーション保存
		mBeforeFrame = mFrame;
		mBeforeMotionType = mMotionType;

		// ブレンド関係
		mBlendSpeed = blendSpeed;
		mBlendFactor = 0.0f;
	}

	// モーション更新
	mMotionType = type;
	mIsLoop = true;
	mIsStopLastFrame = false;
	mFrame = 0.0f;
}

void SkinnedMesh::SetMotionOnce(int onceType, int nextType, float blendSpeed)
{
	// 両方同じモーションならreturn
	if (mMotionType == onceType && mNextType == nextType) return;

	// 今のモーション保存
	mBeforeFrame = mFrame;
	mBeforeMotionType = mMotionType;

	// モーション更新
	mMotionType = onceType;
	mNextType = nextType;
	mIsLoop = false;
	mIsStopLastFrame = false;
	mFrame = 0.0f;

	// ブレンド関係
	mBlendSpeed = blendSpeed;
	mBlendFactor = 0.0f;
}

void SkinnedMesh::SetMotionStopLastFrame(int type, float blendSpeed)
{
	// 両方同じモーションならreturn
	if (mMotionType == type) return;

	// 今のモーション保存
	mBeforeFrame = mFrame;
	mBeforeMotionType = mMotionType;

	// モーション更新
	mMotionType = type;
	mIsLoop = false;
	mIsStopLastFrame = true;
	mFrame = 0.0f;

	// ブレンド関係
	mBlendSpeed = blendSpeed;
	mBlendFactor = 0.0f;
}

void SkinnedMesh::AddMotion(const char* filename, int type)
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
	mMotionFinished = false;

	auto& nowMotion = mMotion[mMotionType];
	int f = static_cast<int>(mFrame);

	// ループチェック
	if (f >= nowMotion.frameNum - 1)
	{
		if (mIsStopLastFrame)
		{
			mFrame -= elapsedTime * 60;
			mMotionFinished = true;
		}
		else
		{
			if (!mIsLoop)
			{
				SetMotion(mNextType, mBlendSpeed);
				mMotionFinished = true;
			}

			mFrame = 0;
		}
	}
}


