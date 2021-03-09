#include "geometry2d.hlsli"

Texture2D diffuse_map : register(t0);
SamplerState diffuse_sampler : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    return diffuse_map.Sample(diffuse_sampler, pin.texcoord);
}