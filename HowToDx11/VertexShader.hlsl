#include "Header.hlsli"

cbuffer PerApplication : register(b0) {
	matrix projectionMatrix;
}
cbuffer PerFrame : register(b1) {
	matrix viewMatrix;
}
cbuffer PerObject : register(b2) {
	matrix worldMatrix;
}

struct VS_INPUT
{
	float3 vPosition : POSITION;
	float3 color : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
	output.vPosition = mul(mvp, float4(input.vPosition, 1.0f));
	//output.vPosition = float4(input.vPosition, 1.0f);
	output.color = float4(input.color, 1.0f);

	return output;
}