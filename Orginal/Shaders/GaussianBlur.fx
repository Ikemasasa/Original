#include "General.fx"

struct PSInput
{
	float4 pos   : SV_POSITION;
	float4 color : COLOR;
	float2 tex   : TEXCOORD0;
};

PSInput VSMain(VSInput input)
{
	PSInput output = (PSInput)0;

	output.pos   = float4(input.pos, 1.0);
	output.color = input.color;
	output.tex   = input.tex;

	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 color = (float4)0;
	color.a = input.color.a;

	[unroll(BUFFER_SIZE)]
	for (int i = 0; i < KarnelSize * KarnelSize; i++) 
	{
		float2 offset = Texcel * Weight[i].xy;
		float weight = Weight[i].z;
		color.rgb += Diffuse.Sample(Sampler, input.tex + offset).rgb * weight;
	}

	return color;
}