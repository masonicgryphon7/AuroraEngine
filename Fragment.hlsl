#define pi 3.14159265359
#define Epsilon 0.0001

struct VS_OUT
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;
	float4 worldPosition : WPOSITION;
	float4 cameraPosition : CAMERAPOSITIOM;
	float3x3 TBNMatrix : TBNMATRIX;
};

cbuffer MATRIX_Buffer :register (b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 cameraPosition;
	bool isTerrain;
};

Texture2D Diffuse:register(t0);
Texture2D NormalTexture:register(t1);
Texture2D AORoughMetTexture:register(t2);
SamplerState sampAni;

Texture2D Grass : register(t3);
Texture2D GrassNormal : register(t4);
Texture2D GrassAORoughMetTexture : register(t5);

Texture2D Mountain : register(t6);
Texture2D MountainNormal : register(t7);
Texture2D MountainAORoughMetTexture : register(t8);

Texture2D Sand : register(t9);
Texture2D SandNormal : register(t10);
Texture2D SandAORoughMetTexture : register(t11);
Texture2D ID_Map : register(t12);

float distributionGGX(float3 normal, float3 halfV, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float normDotHalfV = max(dot(normal, halfV), 0.0f);
	float normDotHalfV_2 = normDotHalfV * normDotHalfV;

	float denom = (normDotHalfV_2 * (a2 - 1.0f) + 1.0f);
	denom = pi * denom * denom;

	return a2 / denom;
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


	float3 albedo = pow(Diffuse.Sample(sampAni, input.Uv).xyz, float3(2.2, 2.2, 2.2));
	float3 N = NormalTexture.Sample(sampAni, input.Uv).xyz;
	float3 AORoughMet = AORoughMetTexture.Sample(sampAni, input.Uv).xyz;
	float metallic = AORoughMet.z;//met_Roug_Ao.x;
	float roughness = AORoughMet.y;
	float ao = AORoughMet.x;//met_Roug_Ao.z;	
		
	if (isTerrain)
	{
		float3 IDcolor, colorValue;
		IDcolor = ID_Map.Sample(sampAni, input.Uv).xyz;

		colorValue.x = IDcolor.x;
		colorValue.y = IDcolor.y;
		colorValue.z = IDcolor.z;


		if (colorValue.x > Epsilon) //R
		{
			albedo = lerp(albedo, Grass.Sample(sampAni, input.Uv).xyz, colorValue.x);
			N = lerp(N, GrassNormal.Sample(sampAni, input.Uv).xyz, colorValue.x);
			ao = lerp(ao, GrassAORoughMetTexture.Sample(sampAni, input.Uv).x, colorValue.x);
			metallic = lerp(metallic, GrassAORoughMetTexture.Sample(sampAni, input.Uv).y, colorValue.x);
			roughness = lerp(roughness, GrassAORoughMetTexture.Sample(sampAni, input.Uv).z, colorValue.x);

			//albedo = albedo * float3(1.0f, 0.1, 0.1f);
		}

		if (colorValue.y > Epsilon) //G
		{
			albedo = lerp(albedo, Mountain.Sample(sampAni, input.Uv).xyz, colorValue.y);
			N = lerp(N, MountainNormal.Sample(sampAni, input.Uv).xyz, colorValue.y);
			ao = lerp(ao, MountainAORoughMetTexture.Sample(sampAni, input.Uv).x, colorValue.y);
			metallic = lerp(metallic, MountainAORoughMetTexture.Sample(sampAni, input.Uv).y, colorValue.y);
			roughness = lerp(roughness, MountainAORoughMetTexture.Sample(sampAni, input.Uv).z, colorValue.y);

			//albedo = albedo * float3(0.1f, 1.0, 0.1f);
		}

		if (colorValue.z > Epsilon) //B
		{
			albedo = lerp(albedo, Sand.Sample(sampAni, input.Uv).xyz, colorValue.z);
			N = lerp(N, SandNormal.Sample(sampAni, input.Uv).xyz, colorValue.z);
			ao = lerp(ao, SandAORoughMetTexture.Sample(sampAni, input.Uv).x, colorValue.z);
			metallic = lerp(metallic, SandAORoughMetTexture.Sample(sampAni, input.Uv).y, colorValue.z);
			roughness = lerp(roughness, SandAORoughMetTexture.Sample(sampAni, input.Uv).z, colorValue.z);

			//albedo = albedo * float3(0.1f, 0.1, 1.0f);
		}


	}

	N = N * 2.0f - 1.0f;
	N = normalize(mul(N, input.TBNMatrix));

	float3 V = normalize(input.cameraPosition.xyz - input.worldPosition.xyz);
	normalize(input.cameraPosition.xyz - input.worldPosition.xyz);

	//light value
	float3 lightDirection = normalize(float3(1, 3, 1));
	float3 lightColor = float3(1, 1, 1);
	//pbr
	float3 f0 = float3(0.04f, 0.04f, 0.04f);
	f0 = lerp(f0, albedo, metallic);
	float3 H = normalize(V + lightDirection);

	float NDF = distributionGGX(N, H, roughness);
	float G = geometrySmith(N, V, lightDirection, roughness);
	float3 F = fresnelSchlick(max(dot(H, V), 0.0), f0);
	
	float3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0f) * max(dot(N, lightDirection), 0.0f)+ 0.001f;
	float3 spec = nominator / denominator;

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