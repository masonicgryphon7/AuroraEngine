struct VS_OUT
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;
	float3 Normal : NORMAL;
	float4 worldPosition : Position;
	float4 cameraDirection : cameraDirection;
	float3x3 TBNMatrix : TBNMATRIX;
};

Texture2D Diffuse:register(t0);
Texture2D NormalTexture:register(t1);
Texture2D PBRTexture:register(t2);
SamplerState sampAni;
float pi = 3.14159265359;

float distributionGGX(float3 normal, float3 halfV, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float normDotHalfV = max(dot(normal, halfV), 0.0f);
	float normDotHalfV_2 = normDotHalfV * normDotHalfV;

	float nom = a2;
	float denom = (normDotHalfV_2 * (a2 - 1.0f) + 1.0f);
	denom = pi * denom * denom;

	return nom / max(denom, 0.0001f);
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
	return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 1.0f);
}

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 color = /*float3 (0.7f, 0.0f, 0.0f);*/ Diffuse.Sample(sampAni, input.Uv).xyz;
	float3 normal = normalize(mul((2.0f * NormalTexture.Sample(sampAni, input.Uv).xyz - 1.0f), input.TBNMatrix));
	float3 met_Roug_Ao = PBRTexture.Sample(sampAni, input.Uv).xyz;
	float metallic = 0.0f;//met_Roug_Ao.x;
	float roughness = 1.0f - met_Roug_Ao.y;
	float ao = 1.0f;//met_Roug_Ao.z;
	
	float specularIntensity = 160;
	float specularPower = 5;

	/*diff*/
	float3 lightDirection = normalize(float3(1, 2, 1));
	float3 camDirection = normalize(input.cameraDirection.xyz);
	lightDirection = normalize(lightDirection);
	float diffuseValue = saturate(dot(normal, lightDirection));

	
	//spec
	specularIntensity *= roughness;
	specularPower *= roughness;
	float3 halfV = normalize(lightDirection - camDirection);
	float dotHalfV = dot(normal, halfV);
	specularIntensity = pow(saturate(dotHalfV), specularIntensity);
	float3 specular = specularIntensity * specularPower;

	
	//pbr
	float3 f0 = float3(0.04f, 0.04f, 0.04f);
	f0 = lerp(f0, color, metallic);
	
	/*float NDF = distributionGGX(normal, halfV, roughness);
	float G = geometrySmith(normal, -camDirection, lightDirection, roughness);
	
	float3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(normal, -camDirection), 0.0f) * max(dot(normal, lightDirection), 0.0f);
	float3 spec = nominator / max(denominator, 0.001f);*/
	float3 kS = fresnelSchlick(clamp(dot(halfV, -camDirection), 0.0f, 1.0f), f0);
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= 1.0f - metallic;

	float normDotLight = max(dot(normal, lightDirection), 0.0f);
	float3 lo = (kD * color + specular) * normDotLight;
	float3 amb = color * ao * float3(1, 1, 1);
	float3 finalColor = amb * lo;

	finalColor = finalColor / (finalColor + float3(1.0f, 1.0f, 1.0f));
	finalColor = pow(finalColor, float3(1.0f, 1.0f, 1.0f));

	color *= diffuseValue + specular;
	//color *= diffuseValue + float3(0.2f, 0.2f, 0.2f) + specular;
	//finalColor.x * 2.1f, finalColor.y * 2.0f, finalColor.z * 2.1f
	return float4(color, 1.0f);
}