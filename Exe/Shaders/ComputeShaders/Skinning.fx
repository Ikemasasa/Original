cbuffer cb : register(b0)
{
	uint GroupNumX;
	uint GroupNumY;
	uint GroupNumZ;
	uint VerticesNum;
};

struct Vertex
{
	float3 pos;
	float3 normal;
};

struct Weight
{
	int count;
	int boneIndex[4];
	float weight[4];
};

StructuredBuffer<Vertex>   VerticesSource : register(t0);
StructuredBuffer<Weight>   Weights		  : register(t1);
StructuredBuffer<matrix>   KeyMatrices	  : register(t2);

RWByteAddressBuffer OutVertices : register(u0);


[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	uint index = DTid.x;

	if (VerticesNum < index) return;

	// 頂点バッファ(OutVertices)の先頭アドレス
	const uint stride = 72; // 後で定数バッファにする
	const uint fetchAddress = index * stride;

	float4 p = float4(VerticesSource[index].pos.x, VerticesSource[index].pos.y, VerticesSource[index].pos.z, 1.0);
	float3 n = VerticesSource[index].normal;

	float3 skinnedPos = (float3)0;
	float3 skinnedNormal = asfloat(OutVertices.Load3(fetchAddress + 12));

	// Skinning
	for (int i = 0; i < Weights[index].count; ++i)
	{
		int b = Weights[index].boneIndex[i];

		skinnedPos    += (mul(KeyMatrices[b], p) * Weights[index].weight[i]).xyz;
		skinnedNormal += mul((float3x3)KeyMatrices[b], n) * Weights[index].weight[i];
	}

	// 保存
	OutVertices.Store3(fetchAddress + 0, asuint(skinnedPos));
	OutVertices.Store3(fetchAddress + 12, asuint(skinnedNormal));
}