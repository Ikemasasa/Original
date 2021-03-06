#include "General.fx"

cbuffer CBSky : register(b0)
{
	float4 EyePosSky;
	matrix InverseView;
	matrix InverseProj;
};

struct PSInput
{
	float4 pos	    : SV_POSITION;
	float3 worldPos : POSITION;
};

// 視線ベクトルを正距円筒図法にのっとったUV座標に変換
float2 EquirectangularProjection(float3 v)
{
	static const float PI = 3.141592654f;
	float2 tex;
	tex.x = 1.0 - ((1.0 / (2 * PI)) * atan2(v.z, v.x) + 0.5);
	tex.y = -(1.0 / PI) * atan2(v.y, length(v.xz)) + 0.5;

	tex.x = saturate(tex.x);
	tex.y = saturate(tex.y);
	return tex;
}

PSInput VSMain( VSInput2D input )
{
	PSInput output = (PSInput)0;

	output.pos = float4(input.pos.xyz, 1.0);

	// ワールド変換
	float4 p = mul(InverseProj, float4(input.pos.xyz, 1.0));
	output.worldPos = mul(InverseView, p / p.w).xyz;

	return output;
}

float4 PSMain(PSInput input) : SV_TARGET0
{
	// 視線ベクトル算出
	float3 E = normalize(input.worldPos.xyz - EyePosSky.xyz);
	float4 color = Diffuse.Sample(Sampler, EquirectangularProjection(E));
	color.rgb *= 0.05;
	color.rgb = color.rgb / (1.0 + color.rgb);

	return color;
}