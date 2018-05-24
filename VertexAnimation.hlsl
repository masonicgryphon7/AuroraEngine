#define NUMBER_OF_JOINTS 20

struct VS_IN
{
	float3 Position : POSITION;
	float2 Uv : UV;
	float3 Tangent : TANGENT;
	float3 Bitangent : BITANGENT;
	float3 Normal : NORMAL;
	float4 JointIndices : JOINT_INDICES;
	float4 JointWeights : JOINT_WEIGHTS;

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

	int animate;
};

cbuffer INSTANCE_Buffer :register (b1)
{
	matrix instanceWorld[100];
}; 

cbuffer SKELETON_Buffer :register (b2)
{
	matrix skeletonTransform[20];
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
	output.Position = float4(0, 0, 0, 0);
	if (animate==1) {
		if(input.JointWeights.x>0)
			output.Position = mul(input.JointWeights.x, mul(float4(input.Position, 1), skeletonTransform[input.JointIndices.x]));
		if (input.JointWeights.y>0)
			output.Position = mul(input.JointWeights.y, mul(float4(input.Position, 1), skeletonTransform[input.JointIndices.y]));
		if (input.JointWeights.z>0)
			output.Position = mul(input.JointWeights.z, mul(float4(input.Position, 1), skeletonTransform[input.JointIndices.z]));
		if (input.JointWeights.w>0)
			output.Position = mul(input.JointWeights.w, mul(float4(input.Position, 1), skeletonTransform[input.JointIndices.w]));

		//output.Position = output.Position / output.Position.w;
	}
	else {
		output.Position = float4(input.Position, 1);
	}

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