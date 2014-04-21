#include "GraphicShaderHelper.fx"

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD;
};


VS_INPUT VS( VS_INPUT input )
{
	return input;
}