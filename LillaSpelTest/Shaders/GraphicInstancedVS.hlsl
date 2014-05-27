

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD;
	matrix WorldMatrix : WORLDMATRIX;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD;
};

VS_OUTPUT VS( VS_INPUT input )
{
	VS_OUTPUT output;

	output.Position = mul(float4(input.Position, 1), input.WorldMatrix);
	output.Normal = mul(float4(input.Normal, 0), input.WorldMatrix).xyz;
	output.Tex = input.Tex;

	return output;
}