#include "GeneralDeferred.fx"

float4 PSMain(PSInput input) : SV_Target0
{
	float4 tex = GBufferColor.Sample(Sampler, input.tex);
	float3 P = GBufferPos.Sample(Sampler, input.tex).xyz;
	float3 N = GBufferNormal.Sample(Sampler, input.tex).xyz;
	float3 E = normalize(EyePos.xyz - P);
	float3 L = normalize(LightDir.xyz);
	float3 C = DirColor.rgb;
	float roughness = 0.8; // �z���g�̓e�N�X�`������Ƃ��Ă���
	float metallic = 0.2;


	// N(0,0,0)�̕����͂��̂܂ܕ`��
	if (length(N) <= 0.0001)
	{
		float4 ret;
		ret.rgb = tex.rgb;
		ret.a = 0.0;
		return ret;
	}
	N = normalize(N);

	// roughness����
	float work = 1.0 - roughness;
	work = pow(work, 5.0); // �{�P�
	work = 1.0 - work;

	// ����
	float3 R = normalize(reflect(E, N));
	float2 envUV;
	envUV.x = normalize(R.xz).x;
	envUV.y = R.y * 0.5 + 0.5;
	float3 env = Environment.SampleLevel(SamplerWrap, envUV, work * 9.0).rgb;

	// �g�[���}�b�v
	env *= 0.01;
	env = env / (1.0 + env);
	
	// �t���l��
	float fresnel = dot(E, R);
	fresnel = max(0, fresnel);
	fresnel = pow(fresnel, 5);

	//�g�U����
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(N, L, C, Kd); //�g�U���ˊ֐�
	float3 shadowParam = GetShadowParam(ShadowVP, P);
	if(type == 1) D *= GetVSMFactor(shadowParam.xy, shadowParam.z);

	//���ʔ���
	float3 Ks = float3(0.25, 0.25, 0.25);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 15);//���ʔ��ˊ֐�

	// �u�����h
	float4 color = 0;
	color.rgb += env * 0.05 * (1 - metallic);
	color.rgb += tex.rgb * 1.0 * (1 - metallic);
	color.rgb += env * metallic * tex.rgb;
	color.rgb *= D;
	color.rgb += S;

	// �t���l��(�f�L�e���H
	color.rgb += env * 0.1 * fresnel;

	color.a = 1;
	return color;
}