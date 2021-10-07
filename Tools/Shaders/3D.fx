#include "General.fx"

struct PSInput
{
	float4 pos	    : SV_POSITION;
	float2 tex	    : TEXCOORD;
};

PSInput VSMain(VSInput input)
{
	PSInput output = (PSInput)0;

	float4 projPos = float4(input.pos, 1.0);
	output.pos = mul(WVP, projPos);
	output.tex = input.tex;

	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 albedo = Diffuse.Sample(Sampler, input.tex);
	float4 color = albedo;

	return color;
}