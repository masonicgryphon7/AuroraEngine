#define pi 3.14159265359
struct VS_OUT
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;
	float3 Normal : NORMAL;
	float4 worldPosition : POSITION;
	float4 cameraPosition : CAMERAPOSITIOM;
	float3x3 TBNMatrix : TBNMATRIX;
};

Texture2D Diffuse:register(t0);
Texture2D NormalTexture:register(t1);
Texture2D PBRTexture:register(t2);
SamplerState sampAni;

float distributionGGX(float3 normal, float3 halfV, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float normDotHalfV = max(dot(normal, halfV), 0.0f);
	float normDotHalfV_2 = normDotHalfV * normDotHalfV;

	float denom = (normDotHalfV_2 * (a2 - 1.0f) + 1.0f);
	denom = pi * denom * denom;

	return a2 / max(denom, 0.0001f);
}

float geometrySchlickGGX(float normDotView, float roughness)
{
	float r = roughness + 1.0f;
	float k = (r*r) / 8.0f;

	float nom = normDotView;
	float denom = normDotView * (1.0f - k) + k;

	return nom / denom;
}

float geometrySmith(float3 normal, float3 view, float3 light, float roughness)
{
	float normDotView = max(dot(normal, view), 0.0f);
	float normDotLight = max(dot(normal, light), 0.0f);
	float ggx2 = geometrySchlickGGX(normDotView, roughness);
	float ggx1 = geometrySchlickGGX(normDotLight, roughness);

	return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
	return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 albedo = /*float3 (0.7f, 0.0f, 0.0f);*/ Diffuse.Sample(sampAni, input.Uv).xyz;
	float3 N = NormalTexture.Sample(sampAni, input.Uv).xyz;
	N = N * 2.0 - 1.0;
	N =	normalize(mul(N,input.TBNMatrix));
	float3 met_Roug_Ao = PBRTexture.Sample(sampAni, input.Uv).xyz;
	float metallic = 0.0f;//met_Roug_Ao.x;
	float roughness = 1.0f;
	float ao = 1.0f;//met_Roug_Ao.z;

	float3 V = normalize(input.cameraPosition - input.worldPosition);
	
	//light value
	float3 lightDirection = normalize(float3(1, 3, 1));
	float3 lightColor = float3(1, 1, 1);
	//pbr
	float3 f0 = float3(0.04f, 0.04f, 0.04f);
	f0 = lerp(f0, albedo, metallic);
	float3 H = normalize(V + lightDirection);

	float NDF = distributionGGX(N, H, roughness);
	float G = geometrySmith(N, V, lightDirection, roughness);
	float3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), f0);
	
	float3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0f) * max(dot(N, lightDirection), 0.0f);
	float3 spec = nominator / max(denominator, 0.001f);

	float3 kS = F;
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= 1.0f - metallic;

	float normDotLight = max(dot(N, lightDirection), 0.0f);
	float3 lo = (kD * albedo /pi + spec) * normDotLight*lightColor;
	float3 amb = albedo * ao * float3(0.03f, 0.03f, 0.03f);
	float3 finalColor = amb + lo;

	finalColor = finalColor / (finalColor + float3(1.0f, 1.0f, 1.0f));

	float value = 1.0f / 2.2f;
	finalColor = pow(finalColor, float3(value, value, value));

	return float4(finalColor, 1.0f);
}