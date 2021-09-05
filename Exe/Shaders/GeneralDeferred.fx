// テクスチャ / サンプラー
Texture2D GBufferColor : register(t0);
Texture2D GBufferNormal: register(t1);
Texture2D GBufferPosition : register(t2);
SamplerState Sapmler : register(s0);

// 定数バッファ
cbuffer CBForFrame : register(b0)
{
	float4  EyePos;			//カメラ座標
}

cbuffer CBForLight : register(b1)
{
	float4	LightDir;		//ライトの方向
	float4	LightColor;		//ライトの色
}

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
	float D = dot(N, -L);
	D = max(0, D);			// 負の値を０にする
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

	float3 S = dot(H, N);
	S = max(0, S);
	S = pow(S, Power);
	S = S * K * C;
	return S;
}