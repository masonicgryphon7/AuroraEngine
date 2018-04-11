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
	float4 cameraDirection;
};

struct VS_OUT
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;
	float3 Normal : NORMAL;
	float4 worldPosition : Position;
	float4 cameraDirection : cameraDirection;
	float3x3 TBNMatrix : TBNMATRIX;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.Position = float4(input.Position, 1);
	output.Position = mul(output.Position, world);
	output.worldPosition = mul(output.Position, world);
	output.Position = mul(output.Position, view);
	output.Position = mul(output.Position, projection);
	output.Uv = input.Uv;
	output.Normal = input.Normal;
	output.cameraDirection = cameraDirection;


	//mul( world, mul(float3x3(input.Tangent, input.Bitangent, input.Normal), view));
	output.TBNMatrix = float3x3(input.Tangent, input.Bitangent, input.Normal);

	return output;
}