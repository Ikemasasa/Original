#include "General.fx"

struct PSInput
{
	float4 pos		 : SV_POSITION;
	float4 color	 : COLOR0;
	float2 tex		 : TEXCOORD0;
	float3 wPosition : TEXCOORD1; //ワールド座標
	float3 wNormal   : TEXCOORD2; //ワールド法線
	float3 wTangent  : TEXCOORD3; //ワールド接線
	float3 wBinormal : TEXCOORD4; //ワールド従法線
};

struct PSOutput
{
	float4 color    : SV_TARGET0;
	float4 position : SV_TARGET1;
	float4 normal	: SV_TARGET2;
};


PSInput VSMain(VSInput input)
{
	PSInput output = (PSInput)0;
	float4 P = float4(input.pos, 1.0);
	float3 wPos = mul(World, P).xyz;

	float3x3 M = (float3x3)World;
	float3 wN = mul(M, input.normal).xyz;
	wN = normalize(wN);
	float3 wT = mul(M, input.tangent).xyz;
	wT = normalize(wT);
	float3 wB = mul(M, input.binormal).xyz;
	wB = normalize(wB);

	output.color = input.color;

	//
	output.pos = mul(WVP, P);
	output.tex = input.tex;
	output.wPosition = wPos;
	output.wNormal = wN;
	output.wTangent = wT;
	output.wBinormal = -wB;

	return output;
}

PSOutput PSMain(PSInput input)
{
	float4 color = Diffuse.Sample(Sampler, input.tex);

	// ノーマルマップ
	float3 N = Normal.Sample(Sampler, input.tex).xyz;
	{
		N = N * 2.0 - 1.0;
		N = normalize(N);

		// ３軸（前、右、上）を行列化
		float3 NN = normalize(input.wNormal);
		float3 TT = normalize(input.wTangent);
		float3 BB = normalize(input.wBinormal);
		float3x3 M;
		M[0] = TT;
		M[1] = BB;
		M[2] = NN;
		N = mul(N, M); // NormalMapを補正
	}

	PSOutput ret;
	ret.color = color * input.color;
	ret.position = float4(input.wPosition, 1);
	ret.normal = float4(N, 1);

	return ret;
}