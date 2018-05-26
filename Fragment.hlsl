#define pi 3.14159265359
#define Epsilon 0.0001

struct VS_OUT
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;
	float4 worldPosition : WPOSITION;
	float4 lightspacePosition : LPOSITION;
	float4 cameraPosition : CAMERAPOSITIOM;
	float3x3 TBNMatrix : TBNMATRIX;
	uint instanceID : InstanceID;
};

cbuffer MATRIX_Buffer :register (b0)
{
	matrix world;
	matrix view;
	matrix projection;
	matrix lightProjection;
	float4 cameraPosition;
	int isTerrain;
	float xMaterialTile;
	float yMaterialTile;

	int fill[1];
};

cbuffer Manager_Buffer :register (b1)
{
	float4 fireRing;
};

cbuffer INSTANCE_Buffer :register (b2)
{
	matrix instanceWorld[100];
	int4 unitTag[100];
};

Texture2D Diffuse:register(t0);
Texture2D NormalTexture:register(t1);
Texture2D AORoughMetTexture:register(t2);

SamplerState sampAni : register (s0);
SamplerState IDsampler : register (s1);

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

Texture2D Lava_Albedo : register(t13);
Texture2D Lava_Normal : register(t14);
Texture2D Lava_OcclusionRoughnessMetallic : register(t15);

Texture2D ShadowMap : register(t16);

Texture2D TeamIdMap:register(t17);

float distributionGGX(float3 normal, float3 halfV, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float normDotHalfV = max(dot(normal, halfV), 0.0f);
	float normDotHalfV_2 = normDotHalfV * normDotHalfV;

	float denom = (normDotHalfV_2 * (a2 - 1.0f) + 1.0f);
	denom = pi * denom * denom;

	return a2 / max(denom,0.001f);
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


float ShadowCalculations(float4 lightSpacePosition, float3 normal, float3 light) {
	float3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;

	projCoords.x = projCoords.x * 0.5 + 0.5;
	projCoords.y = projCoords.y * -0.5 + 0.5;

	float currentDepth = projCoords.z;
	float bias = 0.005;
	float2 texelSize = 1.0 / 1024;
	float shadow = 0.0;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = ShadowMap.Sample(IDsampler, projCoords.xy +float2(x,y)* float2(texelSize)).x;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9;
	shadow = 1 - shadow;
	return shadow;
}

float4 PS_main(VS_OUT input) : SV_Target
{
	float2 adjustedUV = float2(input.Uv.x*xMaterialTile, input.Uv.y*yMaterialTile);

	float3 albedo = Diffuse.Sample(sampAni, adjustedUV).xyz;
	float3 N = NormalTexture.Sample(sampAni, adjustedUV).xyz;
	float3 AORoughMet = AORoughMetTexture.Sample(sampAni, adjustedUV).xyz;
	float metallic = AORoughMet.z;//met_Roug_Ao.x;
	float roughness = AORoughMet.y;
	float ao = AORoughMet.x;//met_Roug_Ao.z;
	float3 teamIdMap = TeamIdMap.Sample(sampAni, adjustedUV).xyz;

	
	//return float4(teamIdMap.xyz, 1);
	if (unitTag[input.instanceID].x == 1)
	{
		if (teamIdMap.x >= 0.5)
			albedo = float4(0, 0, 0.8, 0);
	}
	if (unitTag[input.instanceID].x == 2)
	{
		if(teamIdMap.x >= 0.5)
			albedo = float4(0.8, 0, 0, 0);
	}


	if (isTerrain==1)
	{
		float3 colorValue = ID_Map.Sample(IDsampler, input.Uv).xyz;


		if (colorValue.x > Epsilon) //R
		{
			albedo = lerp(albedo, Grass.Sample(sampAni, adjustedUV).xyz, colorValue.x);
			N = lerp(N, GrassNormal.Sample(sampAni, adjustedUV).xyz, colorValue.x);
			float3 aoRoughMetX = GrassAORoughMetTexture.Sample(sampAni, adjustedUV);
			ao = lerp(ao, aoRoughMetX.x, colorValue.x);
			metallic = lerp(metallic, aoRoughMetX.z, colorValue.x);
			roughness = lerp(roughness, aoRoughMetX.y, colorValue.x);

		}

		if (colorValue.y > Epsilon) //G
		{
			albedo = lerp(albedo, Mountain.Sample(sampAni, adjustedUV).xyz, colorValue.y);
			N = lerp(N, MountainNormal.Sample(sampAni, adjustedUV).xyz, colorValue.y);
			float3 aoRoughMetY = MountainAORoughMetTexture.Sample(sampAni, adjustedUV);
			ao = lerp(ao, aoRoughMetY.x, colorValue.y);
			metallic = lerp(metallic, aoRoughMetY.z, colorValue.y);
			roughness = lerp(roughness, aoRoughMetY.y, colorValue.y);

		}

		if (colorValue.z > Epsilon) //B
		{
			albedo = lerp(albedo, Sand.Sample(sampAni, adjustedUV).xyz, colorValue.z);
			N = lerp(N, SandNormal.Sample(sampAni, adjustedUV).xyz, colorValue.z);
			float3 aoRoughMetZ = SandAORoughMetTexture.Sample(sampAni, adjustedUV);
			ao = lerp(ao, aoRoughMetZ.x, colorValue.z);
			metallic = lerp(metallic, aoRoughMetZ.z, colorValue.z);
			roughness = lerp(roughness, aoRoughMetZ.y, colorValue.z);

		}

		float3 middle = float3(149, 0, 149);
		float ringDistance = distance(middle, input.worldPosition);
		float diff = saturate(ringDistance - fireRing.x);

		if (diff>0)
		{
			float lavaLerp = diff;
			//return float4(Lava_Albedo.Sample(sampAni, adjustedUV).xyz, 0);
			//adjustedUV.x = mul(sin(mul(fireRing.y, 0.06f) + mul(adjustedUV.x, 0.7f)), 0.3f) + mul(cos(mul(fireRing.y, 0.05f) + mul(adjustedUV.y, 0.5)), 0.5f);
			//adjustedUV.x += newY;

			//adjustedUV.x += lerp(5.5 * fireRing.y, adjustedUV.x, 0.2);
			if (input.worldPosition.x > 150 && input.worldPosition.z > 150)
			{
				adjustedUV.x += fireRing.y;
				adjustedUV.y += fireRing.y;
			}

			if (input.worldPosition.x > 150 && input.worldPosition.z < 150)
			{
				adjustedUV.x += fireRing.y;
				adjustedUV.y -= fireRing.y;
			}

			if (input.worldPosition.x < 150 && input.worldPosition.z > 150)
			{
				adjustedUV.x -= fireRing.y;
				adjustedUV.y += fireRing.y;
			}

			if (input.worldPosition.x < 150 && input.worldPosition.z < 150)
			{
				adjustedUV.x -= fireRing.y;
				adjustedUV.y -= fireRing.y;
			}

			albedo = lerp(albedo, Lava_Albedo.Sample(sampAni, adjustedUV).xyz,lavaLerp) ;
			N = lerp(N, Lava_Normal.Sample(sampAni, adjustedUV).xyz, lavaLerp);
			float3 aoRoughMetLava = Lava_OcclusionRoughnessMetallic.Sample(sampAni, adjustedUV);
			ao = lerp(ao, aoRoughMetLava.x, lavaLerp);
			metallic = lerp(metallic, aoRoughMetLava.z, lavaLerp);
			roughness = lerp(roughness, aoRoughMetLava.y, lavaLerp);
		}


	}

	albedo = pow(albedo, float3(2.2, 2.2, 2.2));
	N = N * 2.0f - 1.0f;
	N = normalize(mul(N, input.TBNMatrix));

	float3 V = normalize(input.cameraPosition.xyz - input.worldPosition.xyz);

	//light value
	float3 lightDirection = normalize(float3(-0.63, 0.77, 0));
	float3 lightColor = float3(1, 0.9, 0.6);
	float intensity = 2;
	//pbr
	float3 f0 = float3(0.04f, 0.04f, 0.04f);
	f0 = lerp(f0, albedo, metallic);
	float3 H = normalize(V + lightDirection);

	float NDF = distributionGGX(N, H, roughness);
	float G = geometrySmith(N, V, lightDirection, roughness);
	float3 F = fresnelSchlick(clamp(dot(H, V), 0.0,1.0f), f0);
	
	float3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0f) * max(dot(N, lightDirection), 0.0f);
	float3 spec = nominator;// / max(denominator, 0.001f);

	float3 kS = F;
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= 1.0f - metallic;

	float shadow = ShadowCalculations(input.lightspacePosition, N,lightDirection);

	float normDotLight = max(dot(N, lightDirection), 0.0f);
	float3 lo = (kD * albedo /pi + spec) * normDotLight*lightColor*intensity*shadow;
	float3 amb = albedo * ao * float3(0.03f, 0.03f, 0.03f);
	float3 finalColor = amb + lo;

	finalColor = finalColor / (finalColor + float3(1.0f, 1.0f, 1.0f));

	float value = 1.0f / 2.2f;
	finalColor = pow(finalColor, float3(value, value, value));

	return float4(finalColor, 1.0f);
}