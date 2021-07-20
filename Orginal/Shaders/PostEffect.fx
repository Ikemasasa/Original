#include "General.fx"

struct PSInput
{
	float4 pos   : SV_POSITION;
	float2 tex   : TEXCOORD0;
	float4 color : COLOR;
};

PSInput VSMain(VSInput2D input)
{
	PSInput output = (PSInput)0;
	output.pos = float4(input.pos, 1);
	output.tex = input.tex;
	output.color = input.color;
	return output;
}

float4 PSMain(PSInput input) : SV_TARGET0
{
	float4 tex = Diffuse.Sample(Sampler, input.tex);
	float4 color = tex;

	//color = 1 - color;
	// 輝度調整
	color.rgb += 0.0;
	color.rgb *= 1.2;

	// コントラスト調整
	// 0: 0.5   1:そのまま
	float contrast = 1.05;
	color = (color - 0.5) * contrast + 0.5;

	// 彩度調整
	// 0:グレースケール
	// 1:そのまま
	// R120 G60 B0 = R60 G60 B60
	//float gray =(color.r+color.g+color.b) / 3; // 形式は正しいけど人間的にはおかしいやつ
	float gray = dot(color.rgb, float3(0.299, 0.587, 0.114));
	float saturation = 1.2;
	color = (color - gray) * saturation + gray;

	// カラー補正
	color.rgb *= float3(0.9, 1.0, 1.2);

	return color;
}