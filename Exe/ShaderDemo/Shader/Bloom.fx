#include "General.fx"

struct PSInput
{
    float4 pos   : SV_POSITION;
    float2 tex   : TEXCOORD0;
};

cbuffer cb : register(b0)
{
    float Threshold;
    float Intensity;
    float2 dummy;
}

PSInput VSMain(VSInput2D input)
{
    PSInput output;
    output.pos = float4(input.pos, 1.0);
    output.tex = input.tex;

    return output;
}

float4 PSMain(PSInput input) : SV_Target0
{
    float4 color = Diffuse.Sample(Sampler, input.tex);

    // �P�x���o
    color.rgb = color.rgb * step(Threshold, color.rgb);
    color.rgb = min(1.0, color.rgb); // 1�ȏ��1��
    color.rgb *= Intensity;                // �܂Ԃ������΍�

    color.a = 1;
    return color;
}