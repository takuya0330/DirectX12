struct VS_IN
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

struct PS_IN
{
    float4 sv_position : SV_Position;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};