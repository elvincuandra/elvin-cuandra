#include "common.hlsl"

struct PS_IN
{
    float4 position : SV_POSITION0;
    float4 diffuse : COLOR0;
    float2 texcoord : TEXCOORD0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    
    if (material.TextureEnable)
    {
        color = tex.Sample(samp, pin.texcoord);
        color *= pin.diffuse;
    }
    else
    {
        color = pin.diffuse;
    }
    
    return color;
}