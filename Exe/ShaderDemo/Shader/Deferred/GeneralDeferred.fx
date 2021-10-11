// テクスチャ / サンプラー
Texture2D GBufferColor  : register(t0);
Texture2D GBufferPos    : register(t1);
Texture2D GBufferNormal : register(t2);
Texture2D Environment	: register(t13);
Texture2D ShadowMap		: register(t14);
Texture2D Ramp			: register(t15);

SamplerState Sampler : register(s0);
SamplerState SamplerWrap : register(s1);

// データフォーマット
struct VSInput
{
	float3 pos	  : POSITION;
	float3 normal : NORMAL;
	float2 tex    : TEXCOORD;
	float4 color  : COLOR;
};

struct PSInput
{
	float4 pos   : SV_POSITION;
	float2 tex   : TEXCOORD0;
};

struct PointLight
{
	float4 pos;		// xyz:ライトの座標, w:range
	float4 color;	// ライトの色
};

struct SpotLight
{
	float4 pos;	  // xyz:座標, w:range
	float4 dir;	  // xyz:方向, w:inner_corn
	float4 color; // xyz:色,   w:outer_corn
};

// 定数バッファ
cbuffer CBPerFrame : register(b0)
{
	float4  EyePos;			//カメラ座標
}

cbuffer CBForDirLight : register(b1)
{
	float4	LightDir;		// ライトの方向
	float4	DirColor;		// ライトの色
}

static const int POINT_LIGHT_MAX = 16;
cbuffer CBForPointLight : register(b1)
{
	PointLight PointLights[POINT_LIGHT_MAX];
}

static const int SPOT_LIGHT_MAX = 4;
cbuffer CBForSpotLight : register(b1)
{
	SpotLight SpotLights[SPOT_LIGHT_MAX];
}

cbuffer CBShadowParam : register(b2)
{
	float Bias;
	float SlopeScaledBias;
	int type;
	float2 dummy;
}

cbuffer CBShadow : register(b5) 
{
	matrix ShadowVP; // シャドウマップ用のView Projection
};

// VS(汎用)
PSInput VSMain(VSInput input)
{
	PSInput output = (PSInput)0;

	output.pos = float4(input.pos, 1);
	output.tex = input.tex;
	return output;
}


// 関数
//--------------------------------------------
//	拡散反射関数
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// K:反射率(0～1.0)

//ランバートシェーディング
float3 Diffuse(float3 N, float3 L, float3 C, float3 K)
{	
	float D = clamp(dot(N, -L) + 0.2, 0.45, 1.0);
	return K * C * D;
}

//--------------------------------------------
//	鏡面反射関数(ブリン・フォン)
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// E:視点方向ベクトル(正規化済み)
// K:反射率(0～1.0)
// Power:ハイライトの強さ(輝き度)
float3 BlinnPhongSpecular(float3 N, float3 L, float3 C, float3 E, float3 K, float Power)
{
	//ハーフベクトル
	float3 H = normalize(-L + E);

	float3 S = max(0, dot(H, N));
	S = pow(S, Power);
	S = S * K * C;
	return S;
}

//--------------------------------------------
//	ポイントライト追加関数
//--------------------------------------------
// P: ピクセルワールド位置
// N: ワールド法線(正規化済み)
// E: 視線方向ベクトル(正規化済み)
// Kd: 拡散反射率
// Ks: 鏡面反射率 
float3 AddPointLight(float3 P, float3 N, float3 E, float3 Kd, float3 Ks)
{
	float3 PL = 0;		   //ポイントライトベクトル
	float3 PC = 0;		   //ポイントライト色
	float influence = 0;   //ポイントライトの影響
	float3 PD = (float3)0; //拡散反射
	float3 PS = (float3)0; //鏡面反射

	for (int i = 0; i < POINT_LIGHT_MAX; ++i)
	{
		PL = P - PointLights[i].pos.xyz;
		float d = length(PL);
		float r = PointLights[i].pos.w;

		// 影響力算出
		influence = saturate(1.0 - d / r);

		PL = normalize(PL);
		PC = PointLights[i].color.rgb;

		//拡散光の加算
		PD += Diffuse(N, PL, PC, Kd) * influence;

		//鏡面光の加算
		PS += BlinnPhongSpecular(N, PL, PC, E, Ks, 5) * influence;
	}

	return (PD + PS);
}

//--------------------------------------------
//	スポットライト追加関数
//--------------------------------------------
// P: ピクセルワールド位置
// N: ワールド法線(正規化済み)
// E: 視線方向ベクトル(正規化済み)
// Kd: 拡散反射率
// Ks: 鏡面反射率
float3 AddSpotLight(float3 P, float3 N, float3 E, float3 Kd, float3 Ks)
{
	float3 SL = (float3)0; // スポットライトベクトル
	float3 SC = (float3)0; // スポットライト色
	float3 SD = (float3)0; // 拡散反射
	float3 SS = (float3)0; // 鏡面反射
	for (int i = 0; i < SPOT_LIGHT_MAX; ++i)
	{
		SL = P - SpotLights[i].pos.xyz;
		float d = length(SL);
		float r = SpotLights[i].pos.w; // range

		//届かないライト除外
		if (d > r) continue;

		float3 SFront = normalize(SpotLights[i].dir.xyz);
		SL = normalize(SL);

		// 正規化されたベクトルの内積=COS
		float angle = dot(SL, SFront);

		// 影響力算出
		float innerCorn = SpotLights[i].dir.w;
		float outerCorn = SpotLights[i].color.w;
		float area = innerCorn - outerCorn;
		float influence = innerCorn - angle;
		influence = saturate(1.0 - influence / area);

		SC = SpotLights[i].color.rgb;
		SD += Diffuse(N, SL, SC, Kd) * influence;
		SS += BlinnPhongSpecular(N, SL, SC, E, Ks, 20) * influence;
	}

	return (SD + SS);
}

// 戻り値: xy:UV, z:距離
float3 GetShadowParam(matrix vp, float3 pos)
{
	float4 wvpPos = float4(pos, 1.0);
	wvpPos = mul(vp, wvpPos);
	wvpPos /= wvpPos.w;

	// -1 ~ 1 -> 0 ~ 1
	wvpPos.y = -wvpPos.y;
	wvpPos.xy = wvpPos.xy * 0.5 + 0.5;

	return wvpPos.xyz;
}

float GetVSMFactor(float2 uv, float dist)
{
	float maxDepthSlope = max(abs(ddx(dist)), abs(ddy(dist)));	// 最大深度傾斜を求める
	float shadowBias = Bias + SlopeScaledBias * maxDepthSlope;
	shadowBias = max(shadowBias, 0.0);

	// r：深度、g:xの2乗
	float2 depth = ShadowMap.Sample(Sampler, uv).rg;
	float variance = max(0.0, depth.y - depth.x * depth.x);
	float md = dist - depth.x;

	// ライトブリーディング対策
	static const float LIGHTBLEEDING_FACTOR = 0.3; // 適当
	float p = variance / (variance + (md * md));
	p = saturate((p - LIGHTBLEEDING_FACTOR) / (1.0 / LIGHTBLEEDING_FACTOR));
	float factor = saturate(max(p, depth.x >= (dist - shadowBias))) * 0.6 + 0.4;

	return factor;
}