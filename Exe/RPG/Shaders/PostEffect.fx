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

#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FXAA_QUALITY__PRESET 10
#include "FXAA.hlsl"

// お試し
#define PIXEL_SIZE float2(1.0 / 1280.0, 1.0 / 720)
#define SUBPIX 1
#define EDGE_THRESHOLD 0.333
#define EDGE_THRESHOLD_MIN 0.0625

float4 FXAA(float2 uv)
{
	FxaaTex InputFXAATex = { Sampler, Diffuse };
	return FxaaPixelShader(
		uv,                         // FxaaFloat2 pos,
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),     // FxaaFloat4 fxaaConsolePosPos,
		InputFXAATex,                   // FxaaTex tex,
		InputFXAATex,                   // FxaaTex fxaaConsole360TexExpBiasNegOne,
		InputFXAATex,                   // FxaaTex fxaaConsole360TexExpBiasNegTwo,
		PIXEL_SIZE,                 // FxaaFloat2 fxaaQualityRcpFrame,
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),     // FxaaFloat4 fxaaConsoleRcpFrameOpt,
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),     // FxaaFloat4 fxaaConsoleRcpFrameOpt2,
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),     // FxaaFloat4 fxaaConsole360RcpFrameOpt2,
		SUBPIX,                     // FxaaFloat fxaaQualitySubpix,
		EDGE_THRESHOLD,             // FxaaFloat fxaaQualityEdgeThreshold,
		EDGE_THRESHOLD_MIN,         // FxaaFloat fxaaQualityEdgeThresholdMin,
		0.0f,                          // FxaaFloat fxaaConsoleEdgeSharpness,
		0.0f,                          // FxaaFloat fxaaConsoleEdgeThreshold,
		0.0f,                          // FxaaFloat fxaaConsoleEdgeThresholdMin,
		FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f)      // FxaaFloat fxaaConsole360ConstDir,
	);
}

float4 ColorGrading(float4 color)
{
	// 輝度調整
	color.rgb += 0.0;
	color.rgb *= 0.95;

	// コントラスト調整
	// 0: 0.5   1:そのまま
	float contrast = 1.1;
	color = (color - 0.5) * contrast + 0.5;

	// 彩度調整
	// 0:グレースケール
	// 1:そのまま
	float saturation = 1.0;
	float gray = dot(color.rgb, float3(0.299, 0.587, 0.114));
	color = (color - gray) * saturation + gray;

	// カラー補正
	color.rgb *= float3(0.9, 1.05, 1.07);

	return color;
}

float4 PSMain(PSInput input) : SV_TARGET0
{
	float4 tex = Diffuse.Sample(Sampler, input.tex);
	float4 color = tex;

	//float4 color = FXAA(input.tex); なんか変になる
	color = ColorGrading(color);


	return color;
}