#include "General.fx"

struct PSInput
{
    float4 pos   : SV_POSITION;
    float4 color : COLOR;
    float2 tex   : TEXCOORD;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.pos   = float4(input.pos, 1.0);
    output.color = input.color;
    output.tex   = input.tex;

    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return Diffuse.Sample(Sampler, input.tex) * input.color;
}