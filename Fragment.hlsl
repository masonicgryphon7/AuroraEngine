struct VS_OUT
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;
	float3 Normal : NORMAL;
	float4 worldPosition : Position;
	float4 cameraDirection : cameraDirection;
};

Texture2D Diffuse:register(t0);
Texture2D NormalTexture:register(t1);
SamplerState sampAni;


float4 PS_main(VS_OUT input) : SV_Target
{
	float3 color = Diffuse.Sample(sampAni, input.Uv).xyz;
	float3 normal = NormalTexture.Sample(sampAni, input.Uv).xyz;
	float specularIntensity = 16
		;
	float specularPower = 1;


	float3 lightDirection = float3(1, 2, 1);
	lightDirection = normalize(lightDirection);


	float3 halfV = normalize(lightDirection - input.cameraDirection.xyz);
	float dotHalfV = dot(input.Normal, halfV);
	specularIntensity = pow(saturate(dotHalfV), specularIntensity);

	float3 specular = specularIntensity * specularPower;// lightDistance;
	//float3 halfValue = normalize(normalize(input.cameraDirection.xyz - input.worldPosition.xyz) - lightDirection);
	//specularIntensity = pow(saturate(dot(halfValue, normal)), specularIntensity);

	float diffuseValue = saturate(dot(input.Normal, lightDirection));
	
	color *= diffuseValue + specular + float3(0.1,0.1,0.1);

	return float4(color, 1.0f);
};