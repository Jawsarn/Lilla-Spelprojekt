

cbuffer PerFrameBuffer : register(c0)
{
	matrix View[4];
	matrix Projection[4];
	matrix ViewProjection[4];
	float4 EyesPos[4];
	float3 fillers;
	uint NumberOfViewports;
}

cbuffer PerObjectBuffer	:register(c1)
{
	matrix World;
	uint typeOfObject; //with types of textures etc
	float3 fillers3;
}



Texture2D diffuseTexture	: register(t0);
Texture2D specularTexture	: register(t1);
Texture2D normalTexture		: register(t2);
Texture2D glowTexture		: register(t3);

SamplerState wrapSampler	: register(s0);