#include "Header.hlsli"

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}