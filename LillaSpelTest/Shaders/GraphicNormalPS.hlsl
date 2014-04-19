#include "GraphicShaderHelper.fx"


struct GS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD;
	uint viewport	: SV_ViewportArrayIndex;
};



float4 PS(GS_OUTPUT input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}