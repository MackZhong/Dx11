#include "Header.hlsli"

struct VS_INPUT
{
    float3 vPosition : POSITION;
    float3 color : COLOR;
};

struct VS_OUTPUT
{
    float3 vPosition : SV_POSITION;
    float3 color : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.vPosition = input.vPosition;
    output.color = input.color;

	return output;
}