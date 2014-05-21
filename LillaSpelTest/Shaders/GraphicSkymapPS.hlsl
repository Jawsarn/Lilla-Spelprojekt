

Texture2D diffuseTexture	: register(t0);
Texture2D normalGlow		: register(t1);
TextureCube cubeMap			: register(t2);


SamplerState wrapSampler	: register(s0);


struct GS_OUTPUT
{
	float4 Position		: SV_POSITION;
	float3 PositionL	: POSITION;
	uint Viewport		: SV_ViewportArrayIndex;
};



float4 PS(GS_OUTPUT input) : SV_TARGET //diffuseSpec target
{
	return cubeMap.Sample(wrapSampler, input.PositionL);
}