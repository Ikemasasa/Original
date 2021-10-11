#include "GeneralDeferred.fx"

float4 PSMain(PSInput input) : SV_Target0
{
	float4 tex = GBufferColor.Sample(Sampler, input.tex);
	float3 P = GBufferPos.Sample(Sampler, input.tex).xyz;
	float3 N = GBufferNormal.Sample(Sampler, input.tex).xyz;
	float3 E = normalize(EyePos.xyz - P);
	float3 L = normalize(LightDir.xyz);
	float3 C = DirColor.rgb;
	float roughness = 0.8; // ホントはテクスチャからとってくる
	float metallic = 0.2;


	// N(0,0,0)の部分はそのまま描画
	if (length(N) <= 0.0001)
	{
		float4 ret;
		ret.rgb = tex.rgb;
		ret.a = 0.0;
		return ret;
	}
	N = normalize(N);

	// roughness調整
	float work = 1.0 - roughness;
	work = pow(work, 5.0); // ボケ具合
	work = 1.0 - work;

	// 環境光
	float3 R = normalize(reflect(E, N));
	float2 envUV;
	envUV.x = normalize(R.xz).x;
	envUV.y = R.y * 0.5 + 0.5;
	float3 env = Environment.SampleLevel(SamplerWrap, envUV, work * 9.0).rgb;

	// トーンマップ
	env *= 0.01;
	env = env / (1.0 + env);
	
	// フレネル
	float fresnel = dot(E, R);
	fresnel = max(0, fresnel);
	fresnel = pow(fresnel, 5);

	//拡散反射
	float3 Kd = float3(1, 1, 1);
	float3 D = Diffuse(N, L, C, Kd); //拡散反射関数
	float3 shadowParam = GetShadowParam(ShadowVP, P);
	if(type == 1) D *= GetVSMFactor(shadowParam.xy, shadowParam.z);

	//鏡面反射
	float3 Ks = float3(0.25, 0.25, 0.25);
	float3 S = BlinnPhongSpecular(N, L, C, E, Ks, 15);//鏡面反射関数

	// ブレンド
	float4 color = 0;
	color.rgb += env * 0.05 * (1 - metallic);
	color.rgb += tex.rgb * 1.0 * (1 - metallic);
	color.rgb += env * metallic * tex.rgb;
	color.rgb *= D;
	color.rgb += S;

	// フレネル(デキテル？
	color.rgb += env * 0.1 * fresnel;

	color.a = 1;
	return color;
}