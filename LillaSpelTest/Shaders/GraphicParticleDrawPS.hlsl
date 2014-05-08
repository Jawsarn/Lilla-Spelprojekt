#include "GraphicParticleHelper.fx"

struct GS_OUTPUT
{
	float4 PosH : SV_Position;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float Age : AGE;
	uint Viewport	: SV_ViewportArrayIndex;
};



float4 PS(GS_OUTPUT input) : SV_TARGET
{
	float4 finalColor;
	
	finalColor = diffuseTexture.Sample(wrapSampler, input.Tex);


	return finalColor;
}
