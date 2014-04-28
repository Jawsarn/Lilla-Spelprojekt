
struct GS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex		: TEXCORD;
	uint viewport	: SV_ViewportArrayIndex;
};
//add the sampler and the texture here

SamplerState wrapSampler	: register(s0);
Texture2D hudTexture	: register(t0);

float4 PS(GS_OUTPUT input) : SV_TARGET
{
	float4 finalColor;
	finalColor = hudTexture.Sample(wrapSampler, input.tex);
	return finalColor;
}