#include "General.fx"

struct PSInput
{
    float4 pos   : SV_POSITION;
    float2 tex   : TEXCOORD;
    float4 color : COLOR;
};

cbuffer cb : register(b0)
{
    int WaveNum;
    float SinFactor;
    float2 dummy;
}

PSInput VSMain(VSInput2D input)
{
    PSInput output;
    output.pos   = float4(input.pos, 1.0);
    output.tex   = input.tex;
    output.color = input.color;

    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return Diffuse.Sample(Sampler, input.tex) * input.color;
}


// ”g–ä
float4 PSWave(PSInput input) : SV_Target0
{
    float2 dist = input.tex - 0.5;
    float len = length(dist);
    // int waveNum = 10;

    // ”g
    float wave = sin((len + 0.5) * (3.14156 * 2.0) * WaveNum + SinFactor);

    // UV‚Ì•Ï‰»—Ê
    float uvOffsetX = dist.x * wave * (1.0 / WaveNum * 0.5);
    float uvOffsetY = dist.y * wave * (1.0 / WaveNum * 0.5);
    
    // ƒTƒ“ƒvƒŠƒ“ƒO
    float4 color = Diffuse.Sample(Sampler, input.tex + float2(uvOffsetX, uvOffsetY)) * input.color;
    
    // –¾‚é‚³(”g‚Ì‚µ‚¸‚ñ‚Å‚é‚Æ‚±‚ë‚ðˆÃ‚­‚·‚é‚½‚ß
    float bright = (wave + 1.0) * 0.5 * 0.9 + 0.9; // 0.9~1;
    color.rgb *= bright;


    return color;
}