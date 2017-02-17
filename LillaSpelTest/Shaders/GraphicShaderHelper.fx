

cbuffer PerFrameBuffer : register(b0)
{
	matrix View[4];
	matrix Projection[4];
	float4 EyePosition[4];
	float3 fillers;
	float NumberOfViewports;
}

cbuffer PerObjectBuffer	:register(b1)
{
	matrix World;
	float3 Color;
	float typeOfObject; //with types of textures etc
}



Texture2D diffuseTexture	: register(t0);
Texture2D normalGlow		: register(t1);

SamplerState wrapSampler	: register(s0);