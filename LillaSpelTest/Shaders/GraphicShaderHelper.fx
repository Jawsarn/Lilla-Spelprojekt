

cbuffer PerFrameBuffer : register(c0)
{
	matrix View;
	matrix Projection;
	matrix ViewProjection;
	float3 EyesPos;
	uint NumberOfViewports;
}

cbuffer PerObjectBuffer	:register(c1)
{
	matrix World;
	uint typeOfObject; //with types of textures etc
	float3 fillers;
}

