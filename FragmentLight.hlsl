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


float4 PS_main(VS_OUT input) : SV_Target
{
	
	return float4(input.Position.xyz, 1.0f);
}