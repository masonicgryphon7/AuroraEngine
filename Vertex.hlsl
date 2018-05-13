struct VS_IN
{
	float3 Position : POSITION;
	float2 Uv : UV;
	float3 Tangent : TANGENT;
	float3 Bitangent : BITANGENT;
	float3 Normal : NORMAL;
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

cbuffer INSTANCE_Buffer :register (b1)
{
	matrix instanceWorld[100];
	float4 unitTag;
}; 

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
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input, uint instanceID : SV_InstanceID)
{
	VS_OUT output = (VS_OUT)0;
	output.instanceID = instanceID;
	output.Position = float4(input.Position, 1);

	output.Position = mul(output.Position, instanceWorld[instanceID]);
	output.TBNMatrix = mul(float3x3(input.Tangent, input.Bitangent, input.Normal), instanceWorld[instanceID]);

	output.worldPosition = output.Position;
	output.lightspacePosition = mul(output.Position, lightProjection);
	output.Position = mul(output.Position, view);
	output.Position = mul(output.Position, projection);
	output.Uv = input.Uv;
	output.cameraPosition = cameraPosition;



	return output;
}