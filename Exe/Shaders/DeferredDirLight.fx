#include "GeneralDeferred.fx"

float4 PSMain(PSInput input) : SV_Target0
{
	float4 tex = GBufferColor.Sample(Sapmler, input.tex);
	float3 P = GBufferPosition.Sample(Sapmler, input.tex).xyz;
	float3 N = GBufferNormal.Sample(Sapmler, input.tex).xyz;
	float3 E = normalize(EyePos.xyz - P);
	float3 L = normalize(LightDir.xyz);

	// �^���ÃX�^�[�g
	float4 color = 0;

	// ����
	float3 R = normalize(reflect(E, N));
	float2 envUV - R.xy * 0.5 + 0.5;
	// float4 env = Environment.SampleLevel(Sampler, envUV, 9);

	// �g�[���}�b�v
	//env *= 0.01;
	//env = env / (0.5 + env);
	//color += tex * env;

	//�g�U����
	float3 C = LightColor.rgb;
	float3 Kd = tex.rgb;
	float3 D = Diffuse(N, L, C, Kd); //�g�U���ˊ֐�

	//���ʔ���
	float3 Ks = float3(1, 1, 1);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 0.5);//���ʔ��ˊ֐�
	color += float4(D + S, 1.0);

	// N(0,0,0)�̕����͂��̂܂ܕ`��
	if (length(N) <= 0.0001) color = tex;

	return color;
}