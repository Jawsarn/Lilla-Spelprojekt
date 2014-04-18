#include "GraphicShaderhelper.fx"

struct VS_INPUT
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

GS_OUTPUT VS( VS_INPUT input )
{
	GS_OUTPUT output;

	output.position = mul(float4(input.position, 1), World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);
	//output.position = float4(input.position,1);
	output.normal = input.normal;
	output.tex = input.tex;
	return output;
}