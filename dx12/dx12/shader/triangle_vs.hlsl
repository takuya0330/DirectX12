#include "triangle.hlsli"

PS_IN main(VS_IN vin)
{
    PS_IN pout;
    pout.sv_position = float4(vin.position, 1);
    pout.color = vin.color;
    return pout;
}