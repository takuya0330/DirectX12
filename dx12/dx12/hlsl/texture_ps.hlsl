#include "Texture.hlsli"

//Texture2D Texture : register(t0);
//SamplerState Sampler : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    return pin.color;
    //return Texture.Sample(Sampler, In.Texcoord) * In.Color;
}