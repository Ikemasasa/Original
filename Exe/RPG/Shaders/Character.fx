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
	// NormalMapを補正
	float3x3 mat;
	mat[0] = normalize(input.normal);
	mat[1] = normalize(input.tangent);
	mat[2] = normalize(input.binormal);
	N = mul(N, mat);

	//	ライティング
	float3 L = normalize(LightDir).xyz;
	float I = -dot(N, L); // Intensity
	// 1021 Rampから明るさをGet！
	float3 ramp = Ramp.Sample(Sampler, float2((I + 1) / 2, 0.5)).rgb;
	float3 LColor = ramp;
	color.rgb *= LColor;

	//// 影
	{
		float2 shadowUV = input.shadowParam.xy;
		// shadowParam(-1 ~ 1)をUV座標(0 ~ 1)に変換
		shadowUV.y = -shadowUV.y;
		shadowUV = shadowUV * 0.5 + 0.5;

		// 最大深度傾斜を求める
		float maxDepthSlope = max(abs(ddx(input.shadowParam.z)), abs(ddy(input.shadowParam.z)));

		float bias = 0.00035; // 固定バイアス
		float slopeScaledBias = 0.001; // 深度傾斜
		float depthBiasClamp = 0.1;  // バイアスクランプ値
		float shadowBias = bias + slopeScaledBias * maxDepthSlope;
		shadowBias = min(shadowBias, depthBiasClamp);

		float minShadowColor = 0.5;
		color.rgb *= GetVSMFactor(shadowUV, input.shadowParam.z - shadowBias, minShadowColor);

	}

	//color.rgb = input.normal;
	return color;
}