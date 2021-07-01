#include "General.fx"

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
	return tex;
}

PSInput VSMain( VSInput input )
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
	float3 E = normalize(input.worldPos.xyz - EyePos.xyz);
	return Diffuse.Sample(Sampler, EquirectangularProjection(E));
}