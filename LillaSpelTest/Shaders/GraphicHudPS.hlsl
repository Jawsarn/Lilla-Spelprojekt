
struct GS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCORD;
	uint viewport	: SV_ViewportArrayIndex;
};
//add the sampler and the texture here

float4 PS(GS_OUTPUT input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}