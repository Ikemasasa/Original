#include "General.fx"

struct PSInput
{
	float4 pos		 : SV_POSITION;
	float4 color	 : COLOR0;
	float3 wPosition : TEXCOORD1; //ワールド座標
	float3 wNormal   : TEXCOORD2; //ワールド法線
	float2 tex		 : TEXCOORD3;
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
	float3 wN = mul((float3x3)World, input.normal).xyz;
	wN = normalize(wN);

	output.color = float4(1, 1, 1, 1);

	//
	output.pos = mul(WVP, P);
	output.wPosition = wPos;
	output.wNormal = wN;
	output.tex = input.tex;

	return output;
}

PSOutput PSMain(PSInput input)
{
	float4 color = Diffuse.Sample(Sampler, input.tex);

	float3 N = normalize(input.wNormal);

	PSOutput ret;
	ret.color = color * input.color;
	ret.position = float4(input.wPosition, 1);
	ret.normal = float4(N, 1);

	return ret;
}