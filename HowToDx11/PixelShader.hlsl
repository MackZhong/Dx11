#include "Header.hlsli"

struct PS_INPUT
{
    float3 color : COLOR;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}