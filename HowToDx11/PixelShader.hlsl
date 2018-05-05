#include "Header.hlsli"
#define MAX_LIGHTS 8

// Light types
#define LIGHT_POINT 0
#define LIGHT_SPOT 1
#define LIGHT_DIRECTIONAL 2

Texture2D tex : register(t0);
sampler samp : register(s0);

struct _Material {
	float4	emissive;	// 16 bytes
	float4	ambient;	// 16 bytes
	float4	diffuse;	// 16 bytes
	float4	specular;	// 16 bytes
	float	specularPower;	// 4 bytes
	bool	useTexture;		// 4 bytes
	float2	padding;		// 8 bytes
};	// total 80 bytes

cbuffer MaterialProperties : register(b0) {
	_Material material;
}	// 5 * 16

struct _Light {
	float4	position;	// 16 bytes
	float4	direction;	// 16 bytes
	float4	color;		// 16 bytes
	float	spotAngle;	// 4 bytes
	float	constantAttenuation;	// 4 bytes
	float	linearAttenuation;		// 4 bytes
	float	quadraticAttenuation;	// 4 bytes
	int		lightType;	// 4 bytes
	bool	enabled;	// 4 bytes
	int2	padding;	// 8 bytes
};	// total 80 bytes

cbuffer LightProperties : register(b1) {
	float4	eyePosition;		// 16 bytes
	float4	globalAmbient;		// 16 bytes
	_Light	lights[MAX_LIGHTS];	// 8 * 80 bytes
}	// total 672 bytes, 42 * 16

float4 Diffuse(_Light light, float3 L, float3 N) {
	return light.color * max(0, dot(N, L));
}
float4 Specular(_Light light, float3 V, float3 L, float3 N) {
	// Phong lighting
	float3 R = normalize(reflect(-L, N));
	float RdotV = max(0, dot(R, V));

	// Blinn-Phong lighting
	//float3 H = normalize(L + V);
	//float NdotH = max(0, dot(N, H));

	return light.color * pow(RdotV, material.specularPower);
}
float Attenuation(_Light light, float d) {
	return 1.0f / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
}
struct _LightingResult {
	float4 diffuse;
	float4 specular;
};

struct _DistanceResult {
	float distance;
	float3 L;
};
_DistanceResult CalcDistance(float4 position, float4 P) {
	_DistanceResult result;
	result.L = (position - P).xyz;
	result.distance = length(result.L);
	result.L = result.L / result.distance;

	return result;
}

_LightingResult PointLight(_Light light, float3 V, float4 P, float3 N) {
	//float3 L = (light.position - P).xyz;
	//float3 distance = length(L);
	//L = L / distance;

	//float attenuation = Attenuation(light, distance);
	_DistanceResult d = CalcDistance(light.position, P);
	float attenuation = Attenuation(light, d.distance);

	_LightingResult result;
	result.diffuse = Diffuse(light, d.L, N) * attenuation;
	result.specular = Specular(light, V, d.L, N) * attenuation;

	return result;
}

_LightingResult DirectionalLight(_Light light, float3 V, float4 P, float3 N) {
	float3 L = -light.direction.xyz;

	_LightingResult result;
	result.diffuse = Diffuse(light, L, N);
	result.specular = Specular(light, V, L, N);

	return result;
}

float SpotCone(_Light light, float3 L) {
	float minCos = cos(light.spotAngle);
	float maxCos = (minCos + 1.0f) / 2.0f;
	float cosAngle = dot(light.direction.xyz, -L);
	return smoothstep(minCos, maxCos, cosAngle);
}

_LightingResult SpotLight(_Light light, float3 V, float4 P, float3 N) {
	//float3 L = (light.position - P).xyz;
	//float distance = length(L);
	//L = L / distance;
	//float attenuation = Attenuation(light, distance);
	_DistanceResult d = CalcDistance(light.position, P);
	float attenuation = Attenuation(light, d.distance);
	float spotIntensity = SpotCone(light, d.L);

	_LightingResult result;
	result.diffuse = Diffuse(light, d.L, N) * attenuation * spotIntensity;
	result.specular = Specular(light, V, d.L, N) * attenuation * spotIntensity;

	return result;
}

_LightingResult ComputeLighting(float4 P, float3 N) {
	float3 V = normalize(eyePosition - P).xyz;
	_LightingResult totalLighting = { { 0, 0, 0, 0}, { 0, 0, 0, 0 } };

	[unroll]
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (!lights[i].enabled) continue;

		_LightingResult result = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };
		if (LIGHT_POINT == lights[i].lightType) {
			result = PointLight(lights[i], V, P, N);
		}
		else if (LIGHT_SPOT == lights[i].lightType) {
			result = SpotLight(lights[i], V, P, N);
		}
		else {
			result = DirectionalLight(lights[i], V, P, N);
		}
		totalLighting.diffuse += result.diffuse;
		totalLighting.specular += result.specular;
	}

	totalLighting.diffuse = saturate(totalLighting.diffuse);
	totalLighting.specular = saturate(totalLighting.specular);

	return totalLighting;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_TARGET{
_LightingResult lighting = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };
//_LightingResult lighting = ComputeLighting(input.positionWorld, normalize(input.normalWorld));
float4 emissive = material.emissive;
float4 ambient = material.ambient * globalAmbient;
float4 diffuse = material.diffuse * lighting.diffuse;
float4 specular = material.specular * lighting.specular;

float4 texColor = { 1, 1, 1, 1 };

if (material.useTexture) {
	texColor = tex.Sample(samp, input.texCoord);
}

return (emissive + ambient + diffuse + specular) * texColor;
}

float4 wireFrame(VS_OUTPUT input) : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}