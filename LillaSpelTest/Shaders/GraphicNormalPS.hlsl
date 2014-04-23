#include "GraphicShaderHelper.fx"


struct GS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD;
	float2 Depth	: DEPTH;
	uint Viewport	: SV_ViewportArrayIndex;
};

struct PS_OUTPUT
{
	// example like 
	float4 Normal_Depth		:SV_TARGET0;
	float4 DiffuseColor_AO	:SV_TARGET1;
	float4 Specular			:SV_TARGET2;
};


PS_OUTPUT PS(GS_OUTPUT input) : SV_TARGET
{
	PS_OUTPUT output;

	float3 diffuseColor = diffuseTexture.Sample(wrapSampler, input.Tex);
	
	float depth = input.Depth.x / input.Depth.y;

	output.Normal_Depth = float4(input.Normal, depth);
	output.DiffuseColor_AO = float4(diffuseColor, 1);
	output.Specular = float4(1,1,1,1);

	return output;
}