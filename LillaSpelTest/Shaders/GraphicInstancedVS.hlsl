

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD;
};

struct VS_INSTANCED
{
	matrix WorldMatrix : INSTANCED;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD;
};

VS_OUTPUT VS( VS_INPUT input, VS_INSTANCED inputInstance )
{
	VS_OUTPUT output;

	output.Position = mul(float4(input.Position, 1), inputInstance.WorldMatrix);
	output.Normal = input.Normal;
	output.Tex = input.Tex;

	return output;
}