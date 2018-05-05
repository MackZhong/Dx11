//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------


struct VS_OUTPUT
{
	float2 texCoord : TEXCOORD0;
	float4 positionWorld : TEXCOORD1;
	float3 normalWorld : TEXCOORD2;
	float4 vPosition : SV_POSITION;
};

struct PS_INPUT
{
	float2 texCoord : TEXCOORD0;
	float4 positionWorld : TEXCOORD1;
	float3 normalWorld : TEXCOORD2;
};
