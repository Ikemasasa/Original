#include "General.fx"

struct PSInput
{
	float4 pos   : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(VSInput input)
{
	PSInput output;
	
	float4 p = float4(input.pos, 1.0);
	output.pos = mul(WVP, p);

	//normal.w = 0;
	//float4 N = normalize(mul(normal, world));
	//float4 L = normalize(-light_direction);

	output.color = MaterialColor;
	output.color.a = MaterialColor.a;
	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}