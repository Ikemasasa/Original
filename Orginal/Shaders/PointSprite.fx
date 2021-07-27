#include "General.fx"

struct VSInputPointSprite
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 size : TEXCOORD;
	float4 color : COLOR;
	float4 param: PARAM;
};

struct GSInput
{
	float3 pos	  : POSITION;
	float3 normal : NORMAL;
	float2 size   : TEXCOORD;
	float4 color  : COLOR;
	float4 param  : PARAM;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
};

cbuffer CBPerFrame : register(b0)
{
	matrix View; // ビュー変換行列
	matrix Projection; // 透視変換行列
};


//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------
GSInput VSMain(VSInputPointSprite input)
{
	GSInput output = (GSInput)0;
	// 出力値設定.
	output.position = input.pos;
	output.normal = input.normal;
	output.color = input.color;
	output.size = input.size;
	output.param = input.param;
	return output;
}


//--------------------------------------------
//	ジオメトリシェーダ
//--------------------------------------------
[maxvertexcount(4)]
void GSMain(point GSInput In[1], inout TriangleStream<PSInput> ParticleStream)
{
	// 座標変換 (ワールド座標系 → ビュー座標系)
	float4 pos = mul(View, float4(In[0].pos, 1.0));
	float2 size = In[0].size;
	// 0604 回転
	float rad = In[0].param.x;
	float c = cos(rad);
	float s = sin(rad);
	float4 right = float4(c, -s, 0, 0);
	float4 up = float4(s, c, 0, 0);
	right *= size.x;
	up *= size.y;
	// 点を面にする(４頂点を作る)
	float4 pos_left_top = pos - right + up;
	float4 pos_left_bottom = pos - right - up;
	float4 pos_right_top = pos + right + up;
	float4 pos_right_bottom = pos + right - up;

	// ベースとなるUV
	int koma = In[0].param.y;
	int koma_u = In[0].param.z;
	int koma_v = In[0].param.w;
	float koma_w = 1.0 / koma_u;
	float koma_h = 1.0 / koma_v;
	float2 baseUV =	float2((koma % koma_u) * koma_w, (koma / koma_v) * koma_h);

	// 左上の点の位置(射影座標系)・UV・色を計算して出力
	PSInput Out = (PSInput)0;
	Out.color = In[0].Color;
	Out.pos = mul(Projection, pos_left_top);
	Out.tex = baseUV + float2(0, 0);
	ParticleStream.Append(Out);

	// 右上の点の位置(射影座標系) とテクスチャ座標の計算をして出力
	Out.color = In[0].color;
	Out.pos = mul(Projection, pos_right_top);
	Out.tex = baseUV + float2(koma_w, 0);
	ParticleStream.Append(Out);

	// 左下の点の位置(射影座標系) とテクスチャ座標の計算をして出力
	Out.color = In[0].color;
	Out.pos = mul(pos, pos_left_bottom);
	Out.Tex = baseUV + float2(0, koma_h);
	ParticleStream.Append(Out);

	// 右下の点の位置(射影座標系) とテクスチャ座標の計算をして出力
	Out.color = In[0].color;
	Out.pos = mul(Projection, pos_right_bottom);
	Out.tex = baseUV + float2(koma_w, koma_h);
	ParticleStream.Append(Out);

	ParticleStream.RestartStrip();
}


//--------------------------------------------
//	ピクセルシェーダー
//--------------------------------------------
float4 PSMain(PSInput input) : SV_TARGET0
{
	float4 color = (float4)0;
	color = Diffuse.Sample(Sapmler, input.tex) * input.color;

	return color;
}