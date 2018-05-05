#include "Header.hlsli"

//struct ColorInput {
//	float4 position : POSITION;
//	float4 color : COLOR;
//};
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}

float4 wireFrame(VS_OUTPUT input) : SV_TARGET
{
    return float4(0.0f, 1.0f, 0.0f, 1.0f);
}