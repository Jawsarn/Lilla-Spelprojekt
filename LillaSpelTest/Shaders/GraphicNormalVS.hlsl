#include "GraphicShaderHelper.fx"

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD;
};


VS_INPUT VS( VS_INPUT input )
{
	return input;
}