#include "GraphicShaderHelper.fx"

struct GS_INPUT
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD;
};

struct GS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD;
};

[maxvertexcount(3)]
void GS( triangle GS_INPUT input[3], inout TriangleStream<GS_OUTPUT> triangleStream )
{
	for (uint i = 0; i < 3; i++)
	{
		GS_OUTPUT output;
		output.position = mul(float4(input[i].position, 1), World);
		output.position = mul(output.position, View);
		output.position = mul(output.position, Projection);
	/*	output.position = mul(output.position, ViewProjection);*/
		output.tex = input[i].tex;
		output.normal = input[i].normal;
		triangleStream.Append(output);
	}
}