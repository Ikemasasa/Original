#include "GeneralDeferred.fx"

float4 PSMain(PSInput input) : SV_Target0
{
	float4 tex = GBufferColor.Sample(Sampler, input.tex);
	float3 P = GBufferPos.Sample(Sampler, input.tex).xyz;
	float3 N = GBufferNormal.Sample(Sampler, input.tex).xyz;
	float3 E = normalize(EyePos.xyz - P);

	N = normalize(N);

	float3 Kd = tex.rgb;
	float3 Ks = float3(1, 1, 1);
	float3 PL = AddPointLight(P, N, E, Kd, Ks);
	return float4(PL, 1.0);
}