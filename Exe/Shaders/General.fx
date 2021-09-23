
// テクスチャ / サンプラー 
Texture2D Diffuse : register(t0);
Texture2D Normal : register(t1);
Texture2D Environment : register(t13);
Texture2D ShadowMap : register(t14);
Texture2D Ramp : register(t15);

SamplerState Sampler	 : register(s0);
SamplerState SamplerWrap : register(s1);


// 定数バッファ
cbuffer CONSTANT_BUFFER : register(b0)
{
	matrix WVP;
	matrix World;
	float4 LightDir;
	float4 MaterialColor;
};

cbuffer CBShadow : register(b5)
{
	matrix ShadowVP; // シャドウマップ用のView Projection
};

cbuffer CB2 : register(b2)
{
	float4 EyePos;
}

// データフォーマット
struct VSInput
{
	float3 pos	    : POSITION;
	float3 normal   : NORMAL;
	float2 tex	    : TEXCOORD;
	float4 color    : COLOR;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
};

struct VSInput2D
{
	float3 pos	  : POSITION;
	float3 normal : NORMAL;
	float2 tex    : TEXCOORD;
	float4 color  : COLOR;
};

float GetVSMFactor(float2 uv, float fragDepth, float minShadowColor)
{
	// r：深度、g:xの2乗
	float2 depth = ShadowMap.Sample(Sampler, uv).rg;

	float variance = max(0.0, depth.y - depth.x * depth.x);
	float md = fragDepth - depth.x;

	// ライトブリーディング対策
	static const float LIGHTBLEEDING_FACTOR = 0.3; // 適当
	float p = variance / (variance + (md * md));
	p = saturate((p - LIGHTBLEEDING_FACTOR) / (1.0 / LIGHTBLEEDING_FACTOR));
	float factor = saturate(max(p, depth.x >= fragDepth));
	
	// 最低値をminShadowColorに合わせる
	factor *= minShadowColor;
	factor += minShadowColor;

	//// 対策なしバージョン
	//float depthSq = depth.x * depth.x;  // E(x)^2
	//float variance = depth.y - depthSq; // σ^2 = E(x^2) - E(x)^2
	//float md = fragDepth - depth.x;
	//float p = variance / (variance + (md * md));
	//float factor = saturate(max(p, depth.x >= fragDepth));

	//// 最低値をminShadowColorに合わせる
	//factor *= minShadowColor;
	//factor += minShadowColor;

	return factor;
}