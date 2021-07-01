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
	float3 B = input.binormal; // 従法線(上)
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

	// ノーマルマップ
	float3 N = Normal.Sample(Sampler, input.tex).xyz;
	{
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
		N = mul(N, M); // NormalMapを補正
	}

	// ライティング
	float3 L = normalize(LightDir).xyz;
	float I = -dot(N, L); // Intensity
	float3 LColor = saturate(I + 0.0);
	color.rgb *= LColor;

	//color.rgb = N;
	//return color;

	// コメント
	{
		//color.rgb = N;
		////1125
		//float3 Eye = input.WorldPosition - ViewPosition;
		//Eye = normalize(Eye);

		//float3 Reflect = reflect(Eye, N);
		//Reflect = normalize(Reflect);

		//float2 env_uv  = Reflect.xy;
		//float2 env_uv2 = Reflect.xz;
		//env_uv2 = normalize(env_uv2);
		//// ベクトルを緯度経度に変換
		//// ここでenv_uvは0 ~ 1になる
		//env_uv.x = acos(env_uv2.x) / 3.1416; // acosは0 ~ 180度が返る
		//env_uv.y = acos(env_uv.y) / 3.1416;
		////env_uv.y = -env_uv.y;

		//// -1 <---> +1 を 0 <---> 1
		////env_uv = env_uv*0.5 + 0.5;
		//env_uv.x *= 0.5;// 360度画像で左端と右端がいっしょだから一回半分にする
		//// 反射が南向きならば右半分を使う(ここで半分にした意味が出る)
		//if (Reflect.z < 0) {
		//	env_uv.x = 1.0 - env_uv.x;
		//}


		// サンプルレベルを下げればざらざら感
		// 映り込み具合を調整すれば金属感
		//float4 env = Environment.Sample(Decale, env_uv);

		//1209
		//float metalic = 0.3;
		//float roughness = 1.0;

		//// レベルをroughness で表す
		//float4 env = Environment.SampleLevel(Decale, env_uv, roughness * 7.0); // MipMap

		// ブレンド
		//color.rgb = (env.rgb * 0.5) * (1.0 - metalic);// 元の素材
		//color.rgb += env.rgb * metalic * albedo.rgb; // 映り込み具合
		//color.rgb += albedo.rgb * (env.rgb + 100) * (1.0 - metalic);
		// とりあえず見えるように嘘
		//color.rgb += albedo * 300.0;

		// 1209 トーンマップは最後　->(本来はposteffect)
		//color.rgb *= 0.01;
		//color.rgb = color.rgb / (1.0 + color.rgb);

	}

	// 影
	{
		float2 shadowUV = input.shadowParam.xy;
		// shadowParam(-1 ~ 1)をUV座標(0 ~ 1)に変換
		shadowUV.y = -shadowUV.y;
		shadowUV = shadowUV * 0.5 + 0.5;

		// 最大深度傾斜を求める
		float maxDepthSlope = max(abs(ddx(input.shadowParam.z)), abs(ddy(input.shadowParam.z)));

		float bias			  = 0.0001; // 固定バイアス
		float slopeScaledBias = 0.001; // 深度傾斜
		float depthBiasClamp  = 0.1;  // バイアスクランプ値

		float shadowBias = bias + slopeScaledBias * maxDepthSlope;
		shadowBias = min(shadowBias, depthBiasClamp);

		float3 minShadowColor = 0.50;
		float shadowThreshold = ShadowMap.SampleCmpLevelZero(ShadowSampler, shadowUV, input.shadowParam.z - shadowBias); // 影にするかの閾値
		color.rgb *= lerp(minShadowColor, float3(1.0, 1.0, 1.0), shadowThreshold);
	}


	return color;
}