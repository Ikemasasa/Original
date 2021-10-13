#include "General.fx"

cbuffer CBGaussianBlur : register(b0)
{
	// �z������܂��ǂ߂Ȃ���������A�����ς��p�ӂ���
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

	float2 offsetLocal = float2(Offset.x, 0.0); // �Е���0�ɂ��邽��

	// ������(���E�����ɂ���̂�weight��0.5�������Ă�)
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 0)).rgb * Weight0; // 1��ōς܂����߂�0.5�͂����ĂȂ�
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 1)).rgb * Weight1 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 2)).rgb * Weight2 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 3)).rgb * Weight3 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 4)).rgb * Weight4 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 5)).rgb * Weight5 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 6)).rgb * Weight6 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 7)).rgb * Weight7 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 8)).rgb * Weight8 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 9)).rgb * Weight9 * 0.5;

	// �E����(�����̃s�N�Z���͍������̎��Ɏ擾���Ă邩�� �P�ƂȂ肩��)
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

	float2 offsetLocal = float2(0.0, Offset.y); // �Е���0�ɂ��邽��

	// �����(�㉺�����ɂ���̂�weight��0.5�������Ă�)
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 0)).rgb * Weight0; // 1��ōς܂����߂�0.5�͂����ĂȂ�
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 1)).rgb * Weight1 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 2)).rgb * Weight2 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 3)).rgb * Weight3 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 4)).rgb * Weight4 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 5)).rgb * Weight5 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 6)).rgb * Weight6 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 7)).rgb * Weight7 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 8)).rgb * Weight8 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex - (offsetLocal.xy * 9)).rgb * Weight9 * 0.5;

	// ������(�����̃s�N�Z���͏�����̎��Ɏ擾���Ă邩�� 1�ƂȂ肩��)
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 1)).rgb * Weight1 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 2)).rgb * Weight2 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 3)).rgb * Weight3 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 4)).rgb * Weight4 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 5)).rgb * Weight5 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 6)).rgb * Weight6 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 7)).rgb * Weight7 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 8)).rgb * Weight8 * 0.5;
	color.rgb += Diffuse.Sample(Sampler, input.tex + (offsetLocal.xy * 9)).rgb * Weight9 * 0.5;

	// �Ō��input.color��������
	color.rgb *= input.color.rgb;
	return color;
}