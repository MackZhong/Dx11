#include "Header.hlsli"

//cbuffer PerApplication : register(b0) {
//	matrix projectionMatrix;
//}

cbuffer PerFrame : register(b0) {
	matrix matViewProjection;
}

cbuffer PerObject : register(b1) {
	matrix matWorld;
	matrix matInverseWorld;
	matrix matWorldViewProjection;
}

struct VS_INPUT {
	// Per-vertex data
	float3 vPosition : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	// Per-instance data
	matrix matWorld : WORLDMATRIX;
	matrix matInverse : INVERSETRANSPOSEWORLDMATRIX;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT input) {
	VS_OUTPUT output;
	//matrix mvp = mul(projectionMatrix, mul(viewMatrix, matWorld));
	//output.vPosition = mul(mvp, float4(input.vPosition, 1.0f));
	//output.vPosition = float4(input.vPosition, 1.0f);
	//output.color = float4(input.color, 1.0f);
	matrix mvp = mul(matViewProjection, input.matWorld);
	float4 posIn = float4(input.vPosition, 1.0f);
	output.vPosition = mul(mvp, posIn);
	output.positionWorld = mul(input.matWorld, posIn);
	output.normalWorld = mul((float3x3)input.matInverse, input.normal);
	output.texCoord = input.texCoord;

	return output;
}