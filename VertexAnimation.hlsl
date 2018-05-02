struct VS_IN
{
	float3 Position : POSITION;
	float2 Uv : UV;
	float3 Tangent : TANGENT;
	float3 Bitangent : BITANGENT;
	float3 Normal : NORMAL;
	float4 JointIndex : JOINTINDEX;
	float4 Weight : WEIGHT;
};

cbuffer MATRIX_Buffer :register (b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 cameraPosition;
	bool isTerrain;
};

cbuffer ANIMATION_POSE_Buffer :register (b1) {
	matrix joints[64];
};


struct VS_OUT
{
	float4 Position : SV_POSITION;
	float2 Uv : UV;
	float4 worldPosition : WPOSITION;
	float4 cameraPosition : CAMERAPOSITIOM;
	float3x3 TBNMatrix : TBNMATRIX;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	//Anim
	output.Position = float4(input.Position, 1);






	//local to projection
	output.Position = mul(output.Position, world);
	output.worldPosition = output.Position;
	output.Position = mul(output.Position, view);
	output.Position = mul(output.Position, projection);
	output.Uv = input.Uv;
	output.cameraPosition = cameraPosition;


	//mul( world, mul(float3x3(input.Tangent, input.Bitangent, input.Normal), view));
	output.TBNMatrix = mul(float3x3(input.Tangent, input.Bitangent, input.Normal), world);

	return output;
}