#include "GeneralDeferred.fx"

float4 PSMain(PSInput input)
{
	float4 color = GBufferColor.Sample(Sapmler, input.tex);
	float3 pos = GBufferPosition.Sample(Sapmler, input.tex).xyz;
	float3 normal = GBufferNormal.Sample(Sapmler, input.tex).xyz;
	float3 E = normalize(EyePos.xyz - pos);
	float3 L = normalize(LightDir.xyz);
	// ŠÂ‹«Œõ
	//float3 A = AmbientColor.rgb;
	//ŠgU”½Ë
	float3 C = LightColor.rgb;
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(normal, L, C, Kd); //ŠgU”½ËŠÖ”
	//‹¾–Ê”½Ë
	float3 Ks = float3(1, 1, 1);
	float3 S = BlinnPhongSpecular(normal, L, C, E, Ks, 20);//‹¾–Ê”½ËŠÖ”
	color *= input.color * float4(/*A +*/ D + S, 1.0);

	return color;
}