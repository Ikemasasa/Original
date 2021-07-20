#include "GeneralDeferred.fx"

float4 PSMain(PSInput input)
{
	float4 color = GBufferColor.Sample(Sapmler, input.tex);
	float3 pos = GBufferPosition.Sample(Sapmler, input.tex).xyz;
	float3 normal = GBufferNormal.Sample(Sapmler, input.tex).xyz;
	float3 E = normalize(EyePos.xyz - pos);
	float3 L = normalize(LightDir.xyz);
	// ����
	//float3 A = AmbientColor.rgb;
	//�g�U����
	float3 C = LightColor.rgb;
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(normal, L, C, Kd); //�g�U���ˊ֐�
	//���ʔ���
	float3 Ks = float3(1, 1, 1);
	float3 S = BlinnPhongSpecular(normal, L, C, E, Ks, 20);//���ʔ��ˊ֐�
	color *= input.color * float4(/*A +*/ D + S, 1.0);

	return color;
}