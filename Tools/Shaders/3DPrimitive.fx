#include "General.fx"

struct PSInput
{
	float4 pos	    : SV_POSITION;
	float4 color	: COLOR;
};

PSInput VSMain(VSInput2D input)
{
	PSInput output;

	float4 projPos = float4(input.pos, 1.0);
	output.pos = mul(WVP, projPos);
	output.color = MaterialColor;

	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}