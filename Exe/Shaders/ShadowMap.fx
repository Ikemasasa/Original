#include "General.fx"

struct PSInput
{
	float4 pos  : SV_POSITION;
	float depth : TEXCOORD0; // カメラからの距離
};

PSInput VSMain(VSInput input)
{
	PSInput output = (PSInput)0;

	float4 P = float4(input.pos, 1.0);
	matrix m = mul(ShadowVP, World);
	float4 projPos = mul(m, P);

	output.pos = projPos;
	output.depth = projPos.z / projPos.w; // カメラの行列(View)をかけた後のZ値はカメラからの距離になる
	return output;
}


float4 PSMain(PSInput input) : SV_TARGET0
{
	float4 color;
	color.rgb = input.depth;
	color.a = 1.0;
	return color;
}