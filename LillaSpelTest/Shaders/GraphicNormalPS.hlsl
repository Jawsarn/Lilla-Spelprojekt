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
	float4 Normal_Depth			:SV_TARGET0;
	float4 DiffuseColor_Spec	:SV_TARGET1;
	float4 Glowmap				:SV_TARGET2;
};


PS_OUTPUT PS(GS_OUTPUT input) : SV_TARGET
{
	PS_OUTPUT output;

	float4 diffuseColorSpecFac = diffuseTexture.Sample(wrapSampler, input.Tex);
	
	float depth = input.Depth.x / input.Depth.y;

	output.Normal_Depth = float4(input.Normal, depth);

	output.DiffuseColor_Spec = diffuseColorSpecFac;

	output.Glowmap = float4(0.25, 0.25, 0.25, 1);

	return output;
}