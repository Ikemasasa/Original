#include "General.fx"

struct PSInput
{
	float4 pos	    : SV_POSITION;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
	float2 tex	    : TEXCOORD;
	float4 shadowParam : TEXCOORD1;
};

PSInput VSMain(VSInput input)
{
	PSInput output;

	float4 projPos = float4(input.pos, 1.0);
	float3x3 M = (float3x3)World;

	float3 N = input.normal; // 法線(前)
	N = mul(M, N);
	N = normalize(N);
	float3 T = input.tangent; // 接線(右)
	T = mul(M, T);
	T = normalize(T);
	float3 B = cross(N, T); // 従法線(上)
	B = normalize(B);

	output.normal = N;
	output.tangent = T;
	output.binormal = -B;

	output.pos = mul(WVP, projPos);
	output.tex = input.tex;

	matrix sm = mul(ShadowVP, World);
	output.shadowParam = mul(sm, projPos);

	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 albedo = Diffuse.Sample(Sampler, input.tex) * MaterialColor;
	float4 color = albedo;

	float3 N = Normal.Sample(Sampler, input.tex).xyz;
	N = N * 2.0 - 1.0;
	N = normalize(N);
	// ３軸（前、右、上）を行列化
	float3 NN = normalize(input.normal);
	float3 TT = normalize(input.tangent);
	float3 BB = normalize(input.binormal);
	float3x3 M;
	M[0] = TT;
	M[1] = BB;
	M[2] = NN;
	//// NormalMapを補正
	N = mul(N, M);
	//color.rgb = input.normal;
	//return color;

	//	ライティング
	float3 L = normalize(LightDir).xyz;
	float I = -dot(N, L); // Intensity
	// 1021 Rampから明るさをGet！
	float3 ramp = Ramp.Sample(Sampler, float2((I + 1) / 2, 0.5)).rgb;
	float3 LColor = ramp;
	color.rgb *= LColor;

	//// 影
	//float2 shadowUV = input.shadowParam.xy;
	//// Screen to UV
	//shadowUV.y = -shadowUV.y;
	//shadowUV = shadowUV * 0.5 + 0.5;
	//
	//float d = ShadowMap.Sample(Sampler, shadowUV).r; // 光に一番近い距離(g, b でも一緒)
	//if (d + 0.003 < input.shadowParam.z)
	//{
	//	color.rgb *= 0.7;
	//}

	//color.rgb = input.normal;
	return color;
}