
// テクスチャ / サンプラー 
Texture2D Diffuse : register(t0);
Texture2D Normal : register(t1);
Texture2D ShadowMap : register(t14);
Texture2D Ramp : register(t15);

SamplerState Sampler				 : register(s0);
SamplerComparisonState ShadowSampler : register(s1);


// 定数バッファ
cbuffer CONSTANT_BUFFER : register(b0)
{
	matrix WVP;
	matrix World;
	float4 LightDir;
	float4 MaterialColor;
};

cbuffer CBShadow : register(b1)
{
	matrix ShadowVP; // シャドウマップ用のView Projection
};

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