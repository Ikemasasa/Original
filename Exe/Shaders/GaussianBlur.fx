#include "General.fx"

cbuffer CBGaussianBlur : register(b0)
{
	// 配列をうまく読めなかったから、いっぱい用意した
	float Weight0;
	float Weight1;
	float Weight2;
	float Weight3;
	float Weight4;
	float Weight5;
	float Weight6;
	float Weight7;
	float Weight8;
	float Weight9;
	float2 Offset;
};

struct PSInput
{
	float4 pos   : SV_POSITION;
	float4 color : COLOR;
	float2 tex   : TEXCOORD0;
};

PSInput VSMain(VSInput2D input)
{
	PSInput output = (PSInput)0;

	output.pos   = float4(input.pos, 1.0);
	output.color = input.color;
	output.tex   = input.tex;

	return output;
}

float4 PSHorizontal(PSInput input) : SV_TARGET
{
	float4 color = (float4)0;
	color.a = input.color.a;

	float2 offsetLocal = float2(Offset.x, 0.0); // 片方を0にするため

	// 左方向(左右両方にするのでweightに0.5をかけてる)
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 0)).rgb * Weight0; // 1回で済ますために0.5はかけてない
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 1)).rgb * Weight1 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 2)).rgb * Weight2 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 3)).rgb * Weight3 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 4)).rgb * Weight4 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 5)).rgb * Weight5 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 6)).rgb * Weight6 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 7)).rgb * Weight7 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 8)).rgb * Weight8 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 9)).rgb * Weight9 * 0.5;

	// 右方向(中央のピクセルは左方向の時に取得してるから １つとなりから)
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 1)).rgb * Weight1 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 2)).rgb * Weight2 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 3)).rgb * Weight3 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 4)).rgb * Weight4 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 5)).rgb * Weight5 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 6)).rgb * Weight6 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 7)).rgb * Weight7 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 8)).rgb * Weight8 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 9)).rgb * Weight9 * 0.5;

	return color;
}

float4 PSVertical(PSInput input) : SV_TARGET
{
	float4 color = (float4)0;
	color.a = input.color.a;

	float2 offsetLocal = float2(0.0, Offset.y); // 片方を0にするため

	// 上方向(上下両方にするのでweightに0.5をかけてる)
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 0)).rgb * Weight0; // 1回で済ますために0.5はかけてない
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 1)).rgb * Weight1 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 2)).rgb * Weight2 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 3)).rgb * Weight3 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 4)).rgb * Weight4 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 5)).rgb * Weight5 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 6)).rgb * Weight6 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 7)).rgb * Weight7 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 8)).rgb * Weight8 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 9)).rgb * Weight9 * 0.5;

	// 下方向(中央のピクセルは上方向の時に取得してるから 1つとなりから)
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 1)).rgb * Weight1 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 2)).rgb * Weight2 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 3)).rgb * Weight3 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 4)).rgb * Weight4 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 5)).rgb * Weight5 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 6)).rgb * Weight6 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 7)).rgb * Weight7 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 8)).rgb * Weight8 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 9)).rgb * Weight9 * 0.5;

	// 最後にinput.colorをかける
	color.rgb *= input.color.rgb;
	return color;
}