struct VS_OUT
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;

};

Texture2D Diffuse:register(t0);
SamplerState sampAni;

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 color = Diffuse.Sample(sampAni, input.Uv).xyz;
	return float4(color, 1.0f);
};